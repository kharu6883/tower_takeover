#include "main.h"

#include "config/motor.h"
#include "config/io.h"

#include "control/macro.h"
#include "control/displayController.h"

const double kP = 0.11;

static int towerMode = 0, lastBtn = 0, lastR = 0, rollerAccel = 28, rollerDecel = 30;

static double target, slewOutput = 0, accel = 9, decel = 20;

static PID rackVar, rollerVar;

static bool isTrack = false, isReset = false;

static Display::RemoteDisplay remote;

void opcontrol() {
	Rack.set_brake_mode(MOTOR_BRAKE_HOLD);
	Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
	Arm.set_current_limit(8000);

	RollerL.set_brake_mode(MOTOR_BRAKE_BRAKE);
	RollerR.set_brake_mode(MOTOR_BRAKE_BRAKE);

	armAsync.resume();

	Slew roller(28, 30);

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

				rackVar.output = pTerm(RACK_UP, rackPot.get_value(), kP);

				if(rackVar.output > rackVar.slewOutput + accel) {
			    rackVar.slewOutput += accel;
			  } else {
			    rackVar.slewOutput = rackVar.output;
			  }

				rack(rackVar.slewOutput);

		  } else {

				rackVar.output = pTerm(RACK_TOWER, rackPot.get_value(), kP + 1);

				if(rackVar.output > rackVar.slewOutput + accel) {
			    rackVar.slewOutput += accel;
			  } else {
			    rackVar.slewOutput = rackVar.output;
			  }

				rack(rackVar.slewOutput);
			}

		} else if(master.get_digital(DIGITAL_L2) && !master.get_digital(DIGITAL_L1)) {

			lastBtn = 2;
			rackVar.output = pTerm(RACK_DOWN, rackPot.get_value(), kP + 0.3);

			if(abs(rackVar.output) > rackVar.slewOutput + accel) {
		    rackVar.slewOutput += accel;
		  } else {
		    rackVar.slewOutput = abs(rackVar.output);
		  }

			rack(-rackVar.slewOutput);

		} else if(rackVar.slewOutput > 0) {
			if(lastBtn == 1) {

			  rackVar.slewOutput -= decel;
				rack(rackVar.slewOutput);

			} else if(lastBtn == 2) {

				rackVar.slewOutput -= decel;
				rack(-rackVar.slewOutput);

			}
		} else {

			rackVar.slewOutput = 0;
			rack(0);

		}

		// Roller
		if(master.get_digital(DIGITAL_R1)) {

			roller.calculate(200);

		} else if(master.get_digital(DIGITAL_R2)) {

			roller.calculate(-200);

		} else {

			roller.calculate(0);

		}

		if(towerMode == 0 || towerMode == 4 || towerMode == 5) ::roller(roller.getOutput());

		// Yeet
		pros::delay(20);
	}
}

void macroTask(void* ignore) {

	double armTarget, tolerance = 3;
	const double kP = 210;

	bool disconnected = false;
	// 1 = Primed, 2 = Bottom Tower, 3 = Mid Tower, 4 = Descore Bottom Tower, 5 = Finalization

	towerMode = 0;

	while(true) {
		if(!master.is_connected()) { disconnected = true; pros::delay(20); continue; }
		if(isReset) { towerMode = 0; isReset = false; disconnected = false; armReset(); pros::delay(20); continue; }
		if(master.get_digital(DIGITAL_L1) && master.get_digital(DIGITAL_L2) && rackPot.get_value() <= 1400 && !disconnected) {
				arm(0);
				towerMode = 1;
		}

		if(master.get_digital_new_press(DIGITAL_B) && rackPot.get_value() <= 1400) towerMode = 4;
		if(master.get_digital(DIGITAL_R1) && master.get_digital(DIGITAL_R2)) towerMode = 0;

		switch(towerMode) {
			case 1: {
				if(!master.get_digital(DIGITAL_L2)) towerMode = 2;
				if(!master.get_digital(DIGITAL_L1)) towerMode = 3;
				break;
			}

			case 2: {
				Arm.set_current_limit(10000);
				Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
				tower(1);
				towerMode = 5;
				remote.setText("Low Tower");
				break;
			}

			case 3: {
				Arm.set_current_limit(10000);
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
				disconnected = false;

				Arm.set_current_limit(10000);
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
