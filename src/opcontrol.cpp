#include "main.h"

#include "kari/control/chassis.h"
#include "kari/control/rack.h"
#include "kari/control/arm.h"
#include "kari/util/misc.h"

#include "kari/displayController.h"

void opcontrol() {
	using namespace io;

	int towerMode = 0, lastPos = 0;
	bool isTrack = false;

	Rack rack;
	Arm arm;

	Display display;

	macro::Slew roller(60, 80); // Accel, Decel
	macro::Slew rackSlew(10, 30, true); // Accel, Decel
	macro::PID rackPID(0.13); // kP

	rack.setBrakeType(MOTOR_BRAKE_HOLD);
	arm.setBrakeType(MOTOR_BRAKE_HOLD);

	LF.set_brake_mode(MOTOR_BRAKE_COAST);
	LB.set_brake_mode(MOTOR_BRAKE_COAST);
	RF.set_brake_mode(MOTOR_BRAKE_COAST);
	RB.set_brake_mode(MOTOR_BRAKE_COAST);

	while (true) {
		LF.move_velocity(master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_RIGHT_X) * 1.5);
		LB.move_velocity(master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_RIGHT_X) * 1.5);
		RF.move_velocity(-master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_RIGHT_X) * 1.5);
		RB.move_velocity(-master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_RIGHT_X) * 1.5);

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

				rackPID.withConst(0.12).calculate(RACK_UP, rack.getPot());
				rackSlew.withLimit(rackPID.getOutput()).calculate(rackPID.getOutput());

		  } else { // Tower Placement
				lastPos = 1;

				rackPID.withConst(0.4).calculate(RACK_TOWER, rack.getPot());
				rackSlew.withLimit(rackPID.getOutput()).calculate(rackPID.getOutput());

			}

		} else if(master.get_digital(DIGITAL_L2) && !master.get_digital(DIGITAL_L1)) { // Goin' Down
			lastPos = 0;

			rackPID.withConst(0.43).calculate(RACK_DOWN, rack.getPot());                   
			rackSlew.withLimit(rackPID.getOutput()).calculate(rackPID.getOutput());

		} else { // Stop

			if(lastPos == 2) {
				rackPID.calculate(RACK_UP, rack.getPot());
			} else if(lastPos == 1) {
				rackPID.calculate(RACK_TOWER, rack.getPot());
			} else if(lastPos == 0) {
				rackPID.calculate(RACK_DOWN, rack.getPot());
			}

			rackSlew.withLimit(rackPID.getOutput()).calculate(0);

		}

		rack.move(rackSlew.getOutput());

		/*--------------------------------
				ARM CONTROL
		--------------------------------*/
		if(master.get_digital_new_press(DIGITAL_DOWN)) { towerMode = 0; arm.zero(); }

		if(master.get_digital(DIGITAL_L1) && master.get_digital(DIGITAL_L2) && rack.getPot() <= 1400) towerMode = 1;

		if(master.get_digital_new_press(DIGITAL_B) && rack.getPot() <= 1400) towerMode = 4;
		if(master.get_digital_new_press(DIGITAL_Y) && rack.getPot() <= 1400) towerMode = 5;
		if(master.get_digital_new_press(DIGITAL_X) && rack.getPot() <= 1400) towerMode = 6;

		if(master.get_digital(DIGITAL_R1) && master.get_digital(DIGITAL_R2)) towerMode = 10;

		switch(towerMode) {
			case 1: {
				if(!master.get_digital(DIGITAL_L2)) towerMode = 2;
				if(!master.get_digital(DIGITAL_L1)) towerMode = 3;
				break;
			}

			case 2: {
				arm.tower(1);
				towerMode = 11;
				break;
			}

			case 3: {
				arm.tower(2);
				towerMode = 11;
				break;
			}

			case 4: {
				arm.tower(3);
				towerMode = 0;
				break;
			}

			case 5: {
				arm.tower(4);
				towerMode = 0;
				break;
			}

			case 6: {
				arm.tower(5);
				towerMode = 0;
				break;
			}

			case 10: {
				arm.move(ARM_BOTTOM, 127);
				towerMode = 11;
				break;
			}

			case 11: {
				if(arm.getState()) { towerMode = 0; break; }
				break;
			}

			default: {
				break;
			}
		}


		/*--------------------------------
				ROLLERS
		--------------------------------*/
		if(master.get_digital(DIGITAL_R1)) {

			roller.calculate(127);

		} else if(master.get_digital(DIGITAL_R2)) {

			roller.calculate(-100);

		} else {

			roller.calculate(0);

		}

		if(towerMode == 0 || towerMode == 4 || towerMode == 5 || towerMode == 6) io::roller(roller.getOutput());

		#ifdef DEBUG
		std::cout << "Rack: " << RackMotor.get_current_draw() << "mA, Arm: " << ArmMotor.get_current_draw() << "mA, RollerL: " << RollerL.get_current_draw() << "mA, RollerR: " << RollerR.get_current_draw() << "mA" << std::endl;
		#endif

		// Yeet
		pros::delay(20);
	}
}
