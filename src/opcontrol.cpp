#include "main.h"

#include "config/motor.h"
#include "config/io.h"

#include "control/macro.h"

const double kP = 0.1;

double slewOutput = 0, rate = 9;

double target, clawTarget;

bool isMacro = false;

void opcontrol() {
	Rack.set_brake_mode(MOTOR_BRAKE_HOLD);
	Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
	RollerL.set_brake_mode(MOTOR_BRAKE_BRAKE);
	RollerR.set_brake_mode(MOTOR_BRAKE_BRAKE);

	armAsync.resume();

	while (true) {
		LF.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 + master.get_analog(ANALOG_RIGHT_X) * 2 + master.get_analog(ANALOG_LEFT_X));
		LB.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 + master.get_analog(ANALOG_RIGHT_X) * 2 - master.get_analog(ANALOG_LEFT_X));
		RF.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 - master.get_analog(ANALOG_RIGHT_X) * 2 - master.get_analog(ANALOG_LEFT_X));
		RB.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 - master.get_analog(ANALOG_RIGHT_X) * 2 + master.get_analog(ANALOG_LEFT_X));

		if(master.get_digital(DIGITAL_L1) && !master.get_digital(DIGITAL_L2)) {

			target = pTerm(3615, rackPot.get_value(), kP);

			if(target > slewOutput + rate) {
		    slewOutput += rate;
		  } else {
		    slewOutput = target;
		  }

			rack(slewOutput);

		} else if(master.get_digital(DIGITAL_L2) && !master.get_digital(DIGITAL_L1)) {

			target = pTerm(900, rackPot.get_value(), kP + 0.2);

			if(abs(target) > slewOutput + rate) {
		    slewOutput += rate;
		  } else {
		    slewOutput = abs(target);
		  }

			rack(-slewOutput);

		} else {

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

void yeet() {

}

void macroTask(void* ignore) {

	double armPos, armTarget, armOutput, tolerance = 2.5;

	const double kP = 150;

	bool isReturn = false;
	bool isRatchet = false;
	int towerMode = 0; // 1 = Top Tower, 2 = Bottom Tower

	while(true) {
		armPos = Arm.get_position();

		if(master.get_digital(DIGITAL_L1) && master.get_digital(DIGITAL_L2)) {
			arm(0);

			towerMode = 0;
			isMacro = true;
		}

		if(master.get_digital(DIGITAL_R1) && master.get_digital(DIGITAL_R2)) isReturn = true;

		if(master.get_digital_new_press(DIGITAL_DOWN)) {
			if(!isRatchet) isRatchet = true;
				else isRatchet = false;
			isReturn = true;
		}

		if(isReturn) {
			towerMode = 0;

			if(!isRatchet) armTarget = pTerm(0, armPos, kP + 150);
				else armTarget = pTerm(0.38, armPos, kP + 150);
			arm(armTarget);

			if(isSettled(armTarget, 3)) {
				arm(0);
				isReturn = false;
				isMacro = false;
			}
		}

		if(isMacro && !master.get_digital(DIGITAL_L1) && towerMode != 2 && !isReturn) {
			towerMode = 1;
			while(true) {
				armTarget = pTerm(0, abs(Arm.get_position()), kP + 50);
				arm(armTarget);

				if(isSettled(armTarget, tolerance)) { arm(0); break; }
				wait(20);
			}

			roller(-1.5, 200);
			while(true) {
				armTarget = pTerm(2.509, abs(Arm.get_position()), kP + 50);
				arm(armTarget);

				if(isSettled(armTarget, tolerance)) { arm(0); isMacro = false; break; }
			}

		} else if(isMacro && !master.get_digital(DIGITAL_L2) && towerMode != 1 && !isReturn) {
			towerMode = 2;

			armTarget = pTerm(2.509, abs(armPos), kP);
			arm(armTarget);

			if(isSettled(armTarget, tolerance)) {
				arm(0);
				isMacro = false;
			}
		}

		wait(20);
	}
}
