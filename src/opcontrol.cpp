#include "main.h"

#include "config/motor.h"
#include "config/io.h"

#include "controller/chassis.h"

#include "controller/misc.h"
#include "controller/displayController.h"

static int towerMode = 0, lastPos = 0;

static bool isTrack = false, isReset = false;

void opcontrol() {
	
	Slew roller(60, 80); // Accel, Decel
	Slew rackSlew(30, 60, true); // Accel, Decel
	PID rackPID(0.13); // kP

	pros::Motor LF(LFPORT, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS),
							LB(LBPORT, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS),
							RF(RFPORT, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS),
							RB(RBPORT, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS);

	Rack.set_brake_mode(MOTOR_BRAKE_HOLD);
	Arm.set_brake_mode(MOTOR_BRAKE_HOLD);

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

		if(master.get_digital_new_press(DIGITAL_DOWN)) setReset(true);

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

		#ifdef DEBUG
		std::cout << "Rack: " << Rack.get_current_draw() << "mA, Arm: " << Arm.get_current_draw() << "mA, RollerL: " << RollerL.get_current_draw() << "mA, RollerR: " << RollerR.get_current_draw() << "mA" << std::endl;
		#endif

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
		if(isReset) { towerMode = 0; isReset = false; disconnected = false; armReset(); pros::delay(20); continue; }
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

				Arm.set_current_limit(2000);
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
