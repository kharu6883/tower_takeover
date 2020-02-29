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

	double kP = 0.12, kP_rev = 0.2;

	bool rel = false;

	Rack rack;
	Arm arm;

	macro::Slew rackSlew(9, 9, true); // Accel, Decel
	macro::PID rackPID( kP ); // kP, kD
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
		if(master.get_digital(DIGITAL_L1) && !master.get_digital(DIGITAL_L2)) { // Goin' Up

			lastPos = 1;
			rackPID.withGain( kP ).calculate(RACK_UP, *rack.getPot());
			rackSlew.withLimit(rackPID.getOutput()).calculate(rackPID.getOutput());

		} else if(master.get_digital(DIGITAL_L2) && !master.get_digital(DIGITAL_L1)) { // Goin' Down

			lastPos = 0;
			rackPID.withGain( kP_rev ).calculate(RACK_DOWN, *rack.getPot());
			rackSlew.withLimit(rackPID.getOutput()).calculate(rackPID.getOutput());

		} else { // Stop

			if(lastPos == 1) {
				rackPID.withGain( kP ).calculate(RACK_UP, *rack.getPot());
			} else {
				rackPID.withGain( kP_rev ).calculate(RACK_DOWN, *rack.getPot());
			}

			rackSlew.withLimit(rackPID.getOutput()).calculate(0);

		}

		rack.move(rackSlew.getOutput());

		/*--------------------------------
				ARM CONTROL
		--------------------------------*/
		if(master.get_digital(DIGITAL_A)) { towerMode = 5; rel = true; }
		if(!master.get_digital(DIGITAL_A) && rel) towerMode = 6;

		if(*rack.getPot() <= 1800) {
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
				towerMode = 10;
				break;
			}

			case 2: {
				arm.tower(2);
				towerMode = 10;
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
				ArmMotor.move(-20);
				break;
			}

			case 6: {
				ArmMotor.move(0);
				rel = false;
				towerMode = 0;
				break;
			}

			case 10: {
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
    if(*rack.getPot()>2500){
			roller.calculate(90);
		}
		else
		{
			roller.calculate(127);
		}

		} else if(master.get_digital(DIGITAL_R2)) {

			roller.calculate(-100);

		} else {

			roller.calculate(0);

		}

		if( towerMode == 0 || ( arm.getMacroState() && *arm.getPos() > 0.6 ) ) io::driveRoller(roller.getOutput());

		// std::cout << "Rack: " << RackMotor.get_current_draw() << "mA, Arm: " << ArmMotor.get_current_draw() << "mA, RollerL: " << RollerL.get_current_draw() << "mA, RollerR: " << RollerR.get_current_draw() << "mA" << std::endl;
		// std::cout << "Rack Output: " << rackSlew.getOutput() << ", Rack PID Output: " << rackPID.getOutput() << std::endl;

		std::cout << "Rack: " << *rack.getPot() << ", Rack Output: " << rackSlew.getOutput() << std::endl;

		// Yeet
		pros::delay(10);
	}
}
