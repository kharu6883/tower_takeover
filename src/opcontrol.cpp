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

	macro::Slew rackSlew(9, 9, true); // Accel, Decel
	macro::PID rackPID(0.11); // kP
	macro::Slew roller(5, 10); // Accel, Decel

	rack.setBrakeType(MOTOR_BRAKE_HOLD);
	arm.setBrakeType(MOTOR_BRAKE_HOLD);

	LF.set_brake_mode(MOTOR_BRAKE_COAST);
	LB.set_brake_mode(MOTOR_BRAKE_COAST);
	RF.set_brake_mode(MOTOR_BRAKE_COAST);
	RB.set_brake_mode(MOTOR_BRAKE_COAST);

	while (true) {
		LF.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 1.58 + master.get_analog(ANALOG_LEFT_X) * 1.58 + master.get_analog(ANALOG_RIGHT_X) * 1.58);
		LB.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 1.58 - master.get_analog(ANALOG_LEFT_X) * 1.58 + master.get_analog(ANALOG_RIGHT_X) * 1.58);
		RF.move_velocity(-master.get_analog(ANALOG_LEFT_Y) * 1.58 + master.get_analog(ANALOG_LEFT_X) * 1.58 + master.get_analog(ANALOG_RIGHT_X) * 1.58);
		RB.move_velocity(-master.get_analog(ANALOG_LEFT_Y) * 1.58 - master.get_analog(ANALOG_LEFT_X) * 1.58 + master.get_analog(ANALOG_RIGHT_X) * 1.58);

		/*--------------------------------
		    RACK
		--------------------------------*/
		if(master.get_digital(DIGITAL_L1) && !master.get_digital(DIGITAL_L2)) {

			lastPos = 1;
			rackPID.calculate(RACK_UP, *rack.getPot());
			rackSlew.withLimit(rackPID.getOutput()).calculate(rackPID.getOutput());

		} else if(master.get_digital(DIGITAL_L2) && !master.get_digital(DIGITAL_L1)) { // Goin' Down

			lastPos = 0;
			rackPID.calculate(RACK_DOWN, *rack.getPot());
			rackSlew.withLimit(rackPID.getOutput()).calculate(rackPID.getOutput());

		} else { // Stop

			if(lastPos == 1) {
				rackPID.calculate(RACK_UP, *rack.getPot());
			} else {
				rackPID.calculate(RACK_DOWN, *rack.getPot());
			}

			rackSlew.withLimit(rackPID.getOutput()).calculate(0);

		}

		rack.move(rackSlew.getOutput());

		/*--------------------------------
				ARM CONTROL
		--------------------------------*/
		if(master.get_digital(DIGITAL_A)) arm.setBrakeType(MOTOR_BRAKE_COAST);
			else arm.setBrakeType(MOTOR_BRAKE_HOLD);

		if(*rack.getPot() <= 1400) {
			if(master.get_digital_new_press(DIGITAL_Y)) towerMode = 1;
			if(master.get_digital_new_press(DIGITAL_RIGHT)) towerMode = 2;
			if(master.get_digital_new_press(DIGITAL_B)) towerMode = 3;
			if(master.get_digital_new_press(DIGITAL_DOWN)) towerMode = 4;
		}

		if(master.get_digital(DIGITAL_R1) && master.get_digital(DIGITAL_R2)) { towerMode = 0; arm.zero(); }

		switch(towerMode) {
			case 0: {
				break;
			}

			case 1: {
				arm.tower(1);
				towerMode = 5;
				break;
			}

			case 2: {
				arm.tower(2);
				towerMode = 5;
				break;
			}

			case 3: {
				arm.tower(3);
				towerMode = 0;
				break;
			}

			case 4: {
				arm.tower(5);
				towerMode = 0;
				break;
			}

			case 5: {
				if(arm.getState()) towerMode = 0;
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

		if( towerMode == 0 || ( arm.getMacroState() && *arm.getPos() > 0.6 ) ) io::driveRoller(roller.getOutput());

		// std::cout << "Rack: " << RackMotor.get_current_draw() << "mA, Arm: " << ArmMotor.get_current_draw() << "mA, RollerL: " << RollerL.get_current_draw() << "mA, RollerR: " << RollerR.get_current_draw() << "mA" << std::endl;
		// std::cout << "Rack Output: " << rackSlew.getOutput() << ", Rack PID Output: " << rackPID.getOutput() << std::endl;

		// std::cout << "Rack: " << *rack.getPot() << " TowerMode: " << towerMode << std::endl;

		// Yeet
		pros::delay(10);
	}
}
