#include "main.h"
#include "config.h"

void opcontrol() {
	Lift.set_brake_mode(MOTOR_BRAKE_HOLD);

	while (true) {
		LF.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 + master.get_analog(ANALOG_RIGHT_X) + master.get_analog(ANALOG_LEFT_X) * 2);
		LB.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 + master.get_analog(ANALOG_RIGHT_X) - master.get_analog(ANALOG_LEFT_X) * 2);
		RF.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 - master.get_analog(ANALOG_RIGHT_X) - master.get_analog(ANALOG_LEFT_X) * 2);
		RB.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 - master.get_analog(ANALOG_RIGHT_X) + master.get_analog(ANALOG_LEFT_X) * 2);

		if(master.get_digital(DIGITAL_L1)) {
			lift(127);
		} else if(master.get_digital(DIGITAL_L2)) {
			lift(-127);
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
