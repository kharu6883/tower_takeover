#include "main.h"

#include "config/motor.h"
#include "config/io.h"

#include "control/macro.h"

const double kP = 0.11;

double slewOutput = 0, accel = 9, decel = 20;
int lastBtn = 0;

double target, clawTarget;

bool isMacro = false;
bool isTrack = false;

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

		if(master.get_digital_new_press(DIGITAL_A)) {
			if(!isTrack) isTrack = true;
				else isTrack = false;
		}

		if(master.get_digital(DIGITAL_L1) && !master.get_digital(DIGITAL_L2)) {
			lastBtn = 1;

			if(!isTrack) {

				target = pTerm(3615, rackPot.get_value(), kP);

				if(target > slewOutput + accel) {
			    slewOutput += accel;
			  } else {
			    slewOutput = target;
			  }

				rack(slewOutput);

		  } else {

				target = pTerm(1650, rackPot.get_value(), kP + 1);

				if(target > slewOutput + accel) {
			    slewOutput += accel;
			  } else {
			    slewOutput = target;
			  }

				rack(slewOutput);
			}

		} else if(master.get_digital(DIGITAL_L2) && !master.get_digital(DIGITAL_L1)) {

			lastBtn = 2;
			target = pTerm(900, rackPot.get_value(), kP + 0.3);

			if(abs(target) > slewOutput + accel) {
		    slewOutput += accel;
		  } else {
		    slewOutput = abs(target);
		  }

			rack(-slewOutput);

		} else if(slewOutput > 0) {
			if(lastBtn == 1) {

			  slewOutput -= decel;
				rack(slewOutput);

			} else if(lastBtn == 2) {

				slewOutput -= decel;
				rack(-slewOutput);

			}
		} else {

			slewOutput = 0;
			rack(0);

		}

		if(master.get_digital(DIGITAL_R1) && !isMacro) {

			roller(200);

		} else if(master.get_digital(DIGITAL_R2) && !isMacro) {

			roller(-100);

		} else if(!isMacro) {

			roller(0);

		}

		// Yeet
		pros::delay(20);
	}
}

void macroTask(void* ignore) {

	double armPos, armTarget, armOutput, tolerance = 8;
  double rollerRot = -0.7, rollerSpeed = 80, rollerWait = 0;
	const double kP = 150;

	bool isReturn = false;
	int towerMode = 0; // 1 = Top Tower, 2 = Bottom Tower

	while(true) {
		armPos = Arm.get_position();

		if(master.get_digital(DIGITAL_L1) && master.get_digital(DIGITAL_L2)) {
			arm(0);

			towerMode = 0;
			isMacro = true;
		}

		if(master.get_digital(DIGITAL_R1) && master.get_digital(DIGITAL_R2)) isReturn = true;

		if(isReturn) {
			towerMode = 0;

			armTarget = pTerm(0, armPos, kP + 150);
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

			roller(rollerRot, rollerSpeed);
			wait(rollerWait);
			while(true) {
				armTarget = pTerm(1.605 * COEFFICIENT_ARM, abs(Arm.get_position()), kP + 100);
				arm(armTarget);

				if(isSettled(armTarget, tolerance + 1)) { arm(0); isMacro = false; break; }
				wait(20);
			}

		} else if(isMacro && !master.get_digital(DIGITAL_L2) && towerMode != 1 && !isReturn) {
			towerMode = 2;

			while(true) {
				armTarget = pTerm(0, abs(Arm.get_position()), kP + 50);
				arm(armTarget);

				if(isSettled(armTarget, tolerance)) { arm(0); break; }
				wait(20);
			}

			roller(rollerRot, rollerSpeed);
			wait(rollerWait);
			while(true) {
				armTarget = pTerm(1.401 * COEFFICIENT_ARM, abs(Arm.get_position()), kP + 100);
				arm(armTarget);

				if(isSettled(armTarget, tolerance + 1)) { arm(0); isMacro = false; break; }
				wait(20);
			}
		}

		pros::delay(20);
	}
}
