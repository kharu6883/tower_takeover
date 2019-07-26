#include "main.h"
#include "control/config.h"

const double kP = 150;

double slewOutput = 0, rate = 9;

double target, clawTarget;

bool isMacro = false;

void armController(void* ignore);

void opcontrol() {
	Rack.set_brake_mode(MOTOR_BRAKE_HOLD);
	Arm.set_brake_mode(MOTOR_BRAKE_HOLD);

	pros::Task armAsync(armController);

	while (true) {
		LF.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 + master.get_analog(ANALOG_RIGHT_X) * 2 + master.get_analog(ANALOG_LEFT_X) * 2);
		LB.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 + master.get_analog(ANALOG_RIGHT_X) * 2 - master.get_analog(ANALOG_LEFT_X) * 2);
		RF.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 - master.get_analog(ANALOG_RIGHT_X) * 2 - master.get_analog(ANALOG_LEFT_X) * 2);
		RB.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 - master.get_analog(ANALOG_RIGHT_X) * 2 + master.get_analog(ANALOG_LEFT_X) * 2);

		if(master.get_digital(DIGITAL_L1) && !isMacro) {

			target = pTerm(3.6, Rack.get_position(), kP - 50);

			if(target > slewOutput + rate) {
		    slewOutput += rate;
		  } else {
		    slewOutput = target;
		  }

			rack(slewOutput);

		} else if(master.get_digital(DIGITAL_L2) && !isMacro) {

			target = pTerm(0, Rack.get_position(), kP + 50);

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

		if(master.get_digital(DIGITAL_R1) && !isMacro) {

			roller(200);

		} else if(master.get_digital(DIGITAL_R2) && !isMacro) {

			roller(-200);

		} else if(!isMacro) {

			roller(0);

		}

		wait(20);
	}
}

void armController(void* ignore) {

	double rackPos, armPos, armTarget, armOutput, tolerance = 0.08;

	const double kP = 150;

	bool isReturn = false;
	int mode = 0; // 1 = Top Tower, 2 = Bottom Tower

	while(true) {
		rackPos = Rack.get_position();
		armPos = Arm.get_position();

		if(master.get_digital(DIGITAL_L1) && master.get_digital(DIGITAL_L2)) {
			rack(0);
			arm(0);
			roller(0);

			isMacro = true;
		}

		if(isMacro && (master.get_digital_new_press(DIGITAL_R1) || master.get_digital_new_press(DIGITAL_R2))) {
			isReturn = true;
		}

		if(isReturn) {
			mode = 0;

			armTarget = pTerm(0, abs(rackPos), kP);
			rack(armTarget);

			armTarget = pTerm(0, abs(armPos), kP);
			arm(armTarget);

			if(rackPos > 0 - tolerance && rackPos < 0 + tolerance && armPos > 0 - tolerance && armPos < 0 + tolerance) {
				rack(0);
				arm(0);
				isMacro = false;
				isReturn = false;
			}
		}

		if(isMacro && !master.get_digital(DIGITAL_L1) && mode != 2 && !isReturn) { // Top Tower
			mode = 1;
			armTarget = pTerm(2.2, abs(rackPos), kP);
			rack(armTarget);

			armTarget = pTerm(4, abs(armPos), kP);
			arm(armTarget);

			if(rackPos > 2.2 - tolerance && rackPos < 2.2 + tolerance && armPos > 4 - tolerance && armPos < 4 + tolerance) {
				rack(0);
				arm(0);

				roller(-200);
				wait(500);
				roller(0);
				isReturn = true;
			}
		} else if(isMacro && !master.get_digital(DIGITAL_L2) && mode != 1 && !isReturn) { // Bottom Tower
			mode = 2;
			armTarget = pTerm(1.212, abs(rackPos), kP);
			rack(armTarget);

			armTarget = pTerm(3.1, abs(armPos), kP);
			arm(armTarget);

			if(rackPos > 1.212 - tolerance && rackPos < 1.212 + tolerance && armPos > 3.1 - 0.05 && armPos < 3.1 + 0.05) {
				rack(0);
				arm(0);

				roller(-200);
				wait(500);
				roller(0);
				isReturn = true;
			}
		}

		wait(20);
	}
}
