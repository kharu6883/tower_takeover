#include "main.h"

#include "config/motor.h"
#include "config/io.h"

#include "control/macro.h"
#include "control/displayController.h"

const double kP = 0.11;

static int towerMode = 0, lastBtn = 0;

static double target, clawTarget, slewOutput = 0, accel = 9, decel = 20;

static bool isTrack = false, isReset = false;

Display::RemoteDisplay remote;

void opcontrol() {
	Rack.set_brake_mode(MOTOR_BRAKE_HOLD);
	Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
	Arm.set_current_limit(8000);

	RollerL.set_brake_mode(MOTOR_BRAKE_BRAKE);
	RollerR.set_brake_mode(MOTOR_BRAKE_BRAKE);

	armAsync.resume();

	while (true) {
		LF.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 + master.get_analog(ANALOG_RIGHT_X) * 2 - master.get_analog(ANALOG_LEFT_X));
		LB.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 + master.get_analog(ANALOG_RIGHT_X) * 2 + master.get_analog(ANALOG_LEFT_X));
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

				target = pTerm(RACK_UP, rackPot.get_value(), kP);

				if(target > slewOutput + accel) {
			    slewOutput += accel;
			  } else {
			    slewOutput = target;
			  }

				rack(slewOutput);

		  } else {

				target = pTerm(RACK_TOWER, rackPot.get_value(), kP + 1);

				if(target > slewOutput + accel) {
			    slewOutput += accel;
			  } else {
			    slewOutput = target;
			  }

				rack(slewOutput);
			}

		} else if(master.get_digital(DIGITAL_L2) && !master.get_digital(DIGITAL_L1)) {

			lastBtn = 2;
			target = pTerm(RACK_DOWN, rackPot.get_value(), kP + 0.3);

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

		if(master.get_digital(DIGITAL_R1) && (towerMode == 0 || towerMode == 5)) {

			roller(200);

		} else if(master.get_digital(DIGITAL_R2) && (towerMode == 0 || towerMode == 5)) {

			roller(-200);

		} else if(towerMode == 0 || towerMode == 5) {

			roller(0);

		}

		// Yeet
		pros::delay(20);
	}
}

void macroTask(void* ignore) {

	double armTarget, tolerance = 3;
	const double kP = 210;

	bool isReturn = false;
	bool disconnected = false;
	// 1 = Primed, 2 = Bottom Tower, 3 = Mid Tower, 4 = Descore Bottom Tower, 5 = Finalization

	while(true) {
		if(!master.is_connected()) { disconnected = true; pros::delay(20); continue; }
		if(isReset) { towerMode = 0; isReset = false; disconnected = false; armReset(); pros::delay(20); continue; }
		if(master.get_digital(DIGITAL_L1) && master.get_digital(DIGITAL_L2) && rackPot.get_value() <= 1400 && !disconnected) {
				arm(0);
				towerMode = 1;
		}

		if(master.get_digital_new_press(DIGITAL_B) && rackPot.get_value() <= 1400) towerMode = 4;
		if(master.get_digital(DIGITAL_R1) && master.get_digital(DIGITAL_R2)) isReturn = true;

		if(isReturn) {
			towerMode = 0;
			disconnected = false;
			remote.setText("Zero");

			Arm.set_current_limit(4000);
			Arm.set_brake_mode(MOTOR_BRAKE_COAST);

			armTarget = pTerm(ARM_BOTTOM, Arm.get_position(), kP);
			arm(armTarget);

			if(isSettled(armTarget, tolerance) || armLimit.get_value()) {
				arm(0);
				Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
				isReturn = false;
			}
		}

		switch(towerMode) {
			case 1: {
				if(!master.get_digital(DIGITAL_L2)) towerMode = 2;
				if(!master.get_digital(DIGITAL_L1)) towerMode = 3;
				break;
			}

			case 2: {
				Arm.set_current_limit(8000);
				Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
				tower(1);
				towerMode = 5;
				remote.setText("Low Tower");
				break;
			}

			case 3: {
				Arm.set_current_limit(8000);
				Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
				tower(2);
				towerMode = 5;
				remote.setText("Mid Tower");
				break;
			}

			case 4: {
				Arm.set_current_limit(10000);
				Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
				tower(3);
				towerMode = 5;
				remote.setText("Descore");
				break;
			}

			case 5: {
				Arm.set_current_limit(50);
				Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
				break;
			}

			default: {
				if(!isReturn) {
					Arm.set_current_limit(5000);
					Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
				}
				break;
			}
		}

		pros::delay(20);
	}
}

void setReset(bool set) { isReset = set; }
