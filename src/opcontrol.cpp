#include "main.h"
#include "config.h"

const double kP = 2;

void opcontrol() {
	LiftL.set_brake_mode(MOTOR_BRAKE_HOLD);
	LiftR.set_brake_mode(MOTOR_BRAKE_HOLD);

	while (true) {
		LF.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 + master.get_analog(ANALOG_RIGHT_X) + master.get_analog(ANALOG_LEFT_X) * 2);
		LB.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 + master.get_analog(ANALOG_RIGHT_X) - master.get_analog(ANALOG_LEFT_X) * 2);
		RF.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 - master.get_analog(ANALOG_RIGHT_X) - master.get_analog(ANALOG_LEFT_X) * 2);
		RB.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 - master.get_analog(ANALOG_RIGHT_X) + master.get_analog(ANALOG_LEFT_X) * 2);

		if(master.get_digital(DIGITAL_L1)) {
			lift(pTerm(3, LiftL.get_position(), 40));
		} else if(master.get_digital(DIGITAL_L2)) {
			lift(pTerm(0, LiftL.get_position(), 40));
		} else {
			lift(0);
		}

		if(master.get_digital(DIGITAL_R1)) {
			flap(127);
		} else if(master.get_digital(DIGITAL_R2)) {
			flap(-127);
		} else {
			flap(0);
		}

		wait(5);
	}
}
