#include "main.h"

#include "config/motor.h"
#include "config/io.h"

#include "control/macro.h"

const double kP = 0.11;

static int lastBtn = 0;

static double target, clawTarget, slewOutput = 0, accel = 9, decel = 20;

static bool isMacro = false, isTrack = false, isReset = false;
static int rackTower = 30, rackDown = 30, rackUp = 0;
void opcontrol() {
	Rack.set_brake_mode(MOTOR_BRAKE_HOLD);
	Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
	Arm.set_current_limit(8000);

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

		if(master.get_digital_new_press(DIGITAL_DOWN)) setReset(true);

		if(master.get_digital(DIGITAL_L1) && !master.get_digital(DIGITAL_L2)) {
			lastBtn = 1;

			if(!isTrack) {

				target = pTerm(rackDown, rackPot.get_value(), kP);

				if(target > slewOutput + accel) {
			    slewOutput += accel;
			  } else {
			    slewOutput = target;
			  }

				rack(slewOutput);

		  } else {

				target = pTerm(rackTower, rackPot.get_value(), kP + 1);

				if(target > slewOutput + accel) {
			    slewOutput += accel;
			  } else {
			    slewOutput = target;
			  }

				rack(slewOutput);
			}

		} else if(master.get_digital(DIGITAL_L2) && !master.get_digital(DIGITAL_L1)) {

			lastBtn = 2;
			target = pTerm(rackUp, rackPot.get_value(), kP + 0.3);

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

			roller(-150);

		} else if(!isMacro) {

			roller(0);

		}

		// Yeet
		pros::delay(20);
	}
}

void macroTask(void* ignore) {

	double armTarget, armOutput, tolerance = 3;
  double rollerRot = -0.8, rollerSpeed = 150, rollerWait = 0;
	const double kP = 210;

	bool isReturn = false;
	int towerMode = 0; // 1 = Top Tower, 2 = Bottom Tower, 3 = Descore Bottom Tower

	while(true) {
		if(isReset) { armReset(); pros::delay(20); continue; }

		if(master.get_digital(DIGITAL_L1) && master.get_digital(DIGITAL_L2) && rackPot.get_value() <= 1400) {
			arm(0);

			towerMode = 0;
			isMacro = true;
		}

		if(master.get_digital_new_press(DIGITAL_B) && rackPot.get_value() <= 1400) towerMode = 3;
		if(master.get_digital(DIGITAL_R1) && master.get_digital(DIGITAL_R2)) isReturn = true;

		if(isReturn) {
			towerMode = 0;

			Arm.set_current_limit(4000);
			Arm.set_brake_mode(MOTOR_BRAKE_COAST);

			armTarget = pTerm(ARM_BOTTOM - 0.03, Arm.get_position(), kP);
			arm(armTarget);

			if(isSettled(armTarget, tolerance) || armLimit.get_value()) {
				arm(0);
				Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
				isMacro = false;
				isReturn = false;
			}
		}

		if(towerMode == 3) { // Descore

			Arm.set_current_limit(10000);
			Arm.set_brake_mode(MOTOR_BRAKE_COAST);

			armTarget = pTerm(ARM_LOW_TOWER_DESCORE, Arm.get_position(), kP+10);
			arm(armTarget);

			if(isSettled(armTarget, tolerance)) { arm(0); towerMode = 0; }

		} else if(isMacro && !master.get_digital(DIGITAL_L1) && towerMode != 2 && !isReturn) { // Mid Tower
			towerMode = 1;

			Arm.set_current_limit(8000);
			Arm.set_brake_mode(MOTOR_BRAKE_COAST);

			while(!isReturn) {
				armTarget = pTerm(ARM_BOTTOM, Arm.get_position(), kP);
				arm(armTarget);

				if(isSettled(armTarget, tolerance) || armLimit.get_value()) { arm(0); break; }
				wait(20);
			}

			roller(rollerRot, rollerSpeed);
			wait(rollerWait);

			while(!isReturn) {
				armTarget = pTerm(ARM_MID_TOWER, Arm.get_position(), kP);
				arm(armTarget);

				if(isSettled(armTarget, tolerance+8)) { arm(0); isMacro = false; break; }
				wait(20);
			}

		} else if(isMacro && !master.get_digital(DIGITAL_L2) && towerMode != 1 && !isReturn) { // Low Tower
			towerMode = 2;

			Arm.set_current_limit(8000);
			Arm.set_brake_mode(MOTOR_BRAKE_COAST);

			while(!isReturn) {
				armTarget = pTerm(ARM_BOTTOM, Arm.get_position(), kP);
				arm(armTarget);

				if(isSettled(armTarget, tolerance) || armLimit.get_value()) { arm(0); break; }
				wait(20);
			}

			roller(rollerRot, rollerSpeed);
			wait(rollerWait);

			while(!isReturn) {
				armTarget = pTerm(ARM_LOW_TOWER, Arm.get_position(), kP);
				arm(armTarget);

				if(isSettled(armTarget, tolerance)) { arm(0); isMacro = false; break; }
				wait(20);
			}
		}

		if(towerMode != 0 && !isReturn) {
			Arm.set_current_limit(5);
			Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
		} else if(!isReturn) {
			Arm.set_current_limit(5);
			Arm.set_brake_mode(MOTOR_BRAKE_BRAKE);
		}

		pros::delay(20);
	}
}

void setReset(bool set) { isReset = set; }
