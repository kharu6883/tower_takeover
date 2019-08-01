#include "main.h"
#include "control/config.h"

const double kP = 0.1;

double slewOutput = 0, rate = 9;

double target, clawTarget;

bool isMacro = false;

void macroTask(void* ignore);

void opcontrol() {
	Rack.set_brake_mode(MOTOR_BRAKE_HOLD);
	Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
	FlapL.set_brake_mode(MOTOR_BRAKE_HOLD);
	FlapR.set_brake_mode(MOTOR_BRAKE_HOLD);

	pros::Task armAsync(macroTask);

	while (true) {
		LF.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 + master.get_analog(ANALOG_RIGHT_X) * 2 + master.get_analog(ANALOG_LEFT_X));
		LB.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 + master.get_analog(ANALOG_RIGHT_X) * 2 - master.get_analog(ANALOG_LEFT_X));
		RF.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 - master.get_analog(ANALOG_RIGHT_X) * 2 - master.get_analog(ANALOG_LEFT_X));
		RB.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 - master.get_analog(ANALOG_RIGHT_X) * 2 + master.get_analog(ANALOG_LEFT_X));

		if(master.get_digital(DIGITAL_L1) && !isMacro) {

			target = pTerm(3615, rackPot.get_value(), kP);

			if(target > slewOutput + rate) {
		    slewOutput += rate;
		  } else {
		    slewOutput = target;
		  }

			rack(slewOutput);

		} else if(master.get_digital(DIGITAL_L2) && !isMacro) {

			target = pTerm(900, rackPot.get_value(), kP + 0.2);

			if(abs(target) > slewOutput + rate) {
		    slewOutput += rate;
		  } else {
		    slewOutput = abs(target);
		  }

			rack(-slewOutput);

		} else if(!isMacro) {

			slewOutput = 0;
			rack(0);

		}

		if(master.get_digital(DIGITAL_R1)) {

			roller(200);

		} else if(master.get_digital(DIGITAL_R2)) {

			roller(-200);

		} else {

			roller(0);

		}

		wait(20);
	}
}

void macroTask(void* ignore) {

	double armPos, armTarget, armOutput, tolerance = 0.08;

	const double kP = 150;

	bool isReturn = false;
	int mode = 0; // 1 = Top Tower, 2 = Bottom Tower

	while(true) {
		armPos = Arm.get_position();

		if(master.get_digital(DIGITAL_L1) && master.get_digital(DIGITAL_L2)) {
			arm(0);
			rack(0);
			roller(0);

			mode = 0;
			isMacro = true;
		}

		if(master.get_digital(DIGITAL_R1) && master.get_digital(DIGITAL_R2)) {
			isReturn = true;
		}

		if(isReturn) {
			mode = 0;

			armTarget = pTerm(0, abs(armPos), kP);
			arm(armTarget);

			if(armPos > 0 - tolerance && armPos < 0 + tolerance) {
				arm(0);
				isReturn = false;
				isMacro = false;
			}
		}

		if(isMacro && !master.get_digital(DIGITAL_L1) && mode != 2 && !isReturn) {
			mode = 1;

			armTarget = pTerm(3.2, abs(armPos), kP); // Top Tower
			arm(armTarget);

			if(isSettled(armTarget, tolerance)) {
				arm(0);
				isMacro = false;
			}
		} else if(isMacro && !master.get_digital(DIGITAL_L2) && mode != 1 && !isReturn) {
			mode = 2;

			armTarget = pTerm(2.509, abs(armPos), kP); // Bottom Tower
			arm(armTarget);

			if(isSettled(armTarget, tolerance)) {
				arm(0);
				isMacro = false;
			}
		}

		wait(20);
	}
}
