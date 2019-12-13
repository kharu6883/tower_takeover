#include "main.h"

#include "config/motor.h"
#include "config/io.h"

#include "control/macro.h"
#include "control/displayController.h"

static int towerMode = 0, lastBtn = 0, lastR = 0, rollerAccel = 28, rollerDecel = 30;

static double target, slewOutput = 0, accel = 9, decel = 20;

static int lastPos;

static bool isTrack = false, isReset = false;

void opcontrol() {
	Rack.set_brake_mode(MOTOR_BRAKE_HOLD);
	Arm.set_brake_mode(MOTOR_BRAKE_HOLD);

	armAsync.resume();

	Slew roller(60, 80); // Accel, Decel
	Slew rackSlew(30, 60, true); // Accel, Decel
	PID rackPID(0.09); // kP
	Arm.set_current_limit(8000);

	RollerL.set_brake_mode(MOTOR_BRAKE_HOLD);
	RollerR.set_brake_mode(MOTOR_BRAKE_HOLD);

	armAsync.resume();

	while (true) {
		LF.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 + master.get_analog(ANALOG_RIGHT_X) * 2 - master.get_analog(ANALOG_LEFT_X) * 2);
		LB.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 + master.get_analog(ANALOG_RIGHT_X) * 2 + master.get_analog(ANALOG_LEFT_X) * 2);
		RF.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 - master.get_analog(ANALOG_RIGHT_X) * 2 - master.get_analog(ANALOG_LEFT_X) * 2);
		RB.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 - master.get_analog(ANALOG_RIGHT_X) * 2 + master.get_analog(ANALOG_LEFT_X) * 2);

		if(master.get_digital(DIGITAL_A)) {
			if(!isTrack) isTrack = true;
				else isTrack = false;
			while(master.get_digital(DIGITAL_A)) pros::delay(20);
		}

		/*--------------------------------
		    RACK
		--------------------------------*/
		if(master.get_digital(DIGITAL_L1) && !master.get_digital(DIGITAL_L2)) {

			if(!isTrack) { // Put up Rack
				lastPos = 2;

				rackPID.withConst(0.13).calculate(RACK_UP, rackPot.get_value());
				rackSlew.withLimit(rackPID.getOutput()).calculate(rackPID.getOutput());

		  } else { // Tower Placement
				lastPos = 1;

				rackPID.withConst(0.4).calculate(RACK_TOWER, rackPot.get_value());
				rackSlew.withLimit(rackPID.getOutput()).calculate(rackPID.getOutput());

			}

		} else if(master.get_digital(DIGITAL_L2) && !master.get_digital(DIGITAL_L1)) { // Goin' Down
			lastPos = 0;

			rackPID.withConst(0.43).calculate(RACK_DOWN, rackPot.get_value());
			rackSlew.withLimit(rackPID.getOutput()).calculate(rackPID.getOutput());

		} else { // Stop

			if(lastPos == 2) {
				rackPID.calculate(RACK_UP, rackPot.get_value());
			} else if(lastPos == 1) {
				rackPID.calculate(RACK_TOWER, rackPot.get_value());
			} else if(lastPos == 0) {
				rackPID.calculate(RACK_DOWN, rackPot.get_value());
			}

			rackSlew.withLimit(rackPID.getOutput()).calculate(0);

		}

		Rack.move_velocity(rackSlew.getOutput());

		/*--------------------------------
				ROLLERS
		--------------------------------*/
		if(master.get_digital(DIGITAL_R1)) {

			roller.calculate(200);

		} else if(master.get_digital(DIGITAL_R2)) {

			roller.calculate(-150);

		} else {

			roller.calculate(0);

		}

		if(towerMode == 0 || towerMode == 4 || towerMode == 5 || towerMode == 6 || towerMode == 420) ::roller(roller.getOutput());

		// Yeet
		pros::delay(20);
	}
}

/*--------------------------------
    ARM
--------------------------------*/
void macroTask(void* ignore) {

	double armTarget, tolerance = 3;
	const double kP = 210;

	bool disconnected = false;

	towerMode = 0;

	while (true) {
		if(!master.is_connected()) { disconnected = true; pros::delay(20); continue; }
		if(master.get_digital_new_press(DIGITAL_DOWN)) { towerMode = 0; isReset = false; disconnected = false; armReset(); pros::delay(20); continue; }
		if(master.get_digital(DIGITAL_L1) && master.get_digital(DIGITAL_L2) && rackPot.get_value() <= 1400 && !disconnected) {
				arm(0);
				towerMode = 1;
		}

		if(master.get_digital_new_press(DIGITAL_B) && rackPot.get_value() <= 1400) towerMode = 4;
		if(master.get_digital_new_press(DIGITAL_Y) && rackPot.get_value() <= 1400) towerMode = 5;
		if(master.get_digital_new_press(DIGITAL_X) && rackPot.get_value() <= 1400) towerMode = 6;

		if(master.get_digital(DIGITAL_R1) && master.get_digital(DIGITAL_R2)) towerMode = 0;

		switch(towerMode) {
			case 1: {
				if(!master.get_digital(DIGITAL_L2)) towerMode = 2;
				if(!master.get_digital(DIGITAL_L1)) towerMode = 3;
				break;
			}

			case 2: {
				Arm.set_current_limit(7000);
				Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
				tower(1);
				towerMode = 420;
				break;
			}

			case 3: {
				Arm.set_current_limit(7000);
				Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
				tower(2);
				towerMode = 420;
				break;
			}

			case 4: {
				Arm.set_current_limit(7000);
				Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
				tower(3);
				towerMode = 420;
				break;
			}

			case 5: {
				Arm.set_current_limit(7000);
				Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
				tower(4);
				towerMode = 420;
				break;
			}

			case 6: {
				Arm.set_current_limit(7000);
				Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
				tower(5);
				towerMode = 420;
				break;
			}

			case 420: {
				Arm.set_current_limit(500);
				Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
				break;
			}

			default: {
				disconnected = false;

				Arm.set_current_limit(5000);
				Arm.set_brake_mode(MOTOR_BRAKE_COAST);

				armTarget = pTerm(ARM_BOTTOM, Arm.get_position(), kP + 400);
				arm(armTarget);

				break;
			}
		}

		pros::delay(20);
	}
}

void setReset(bool set) { isReset = set; }
