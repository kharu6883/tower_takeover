#include "main.h"
#include "config.h"

void opcontrol() {
	while (true) {
		LF.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 + master.get_analog(ANALOG_RIGHT_X) + master.get_analog(ANALOG_LEFT_X) * 2);
		LB.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 + master.get_analog(ANALOG_RIGHT_X) - master.get_analog(ANALOG_LEFT_X) * 2);
		RF.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 - master.get_analog(ANALOG_RIGHT_X) - master.get_analog(ANALOG_LEFT_X) * 2);
		RB.move_velocity(master.get_analog(ANALOG_LEFT_Y) * 2 - master.get_analog(ANALOG_RIGHT_X) + master.get_analog(ANALOG_LEFT_X) * 2);

		if(master.get_digital(DIGITAL_R1)) {
			lift(127);
		} else if(master.get_digital(DIGITAL_R2)) {
			lift(-127);
		}

		wait(5);
	}
}

// void manual() {
// 	if(master.get_digital(DIGITAL_L1) && master.get_digital(DIGITAL_L2) && clawMode == 0) {
// 		ClawL.move_absolute(0.1, 30);
// 		ClawR.move_absolute(0.1, 30);
// 	} else if(master.get_digital(DIGITAL_L1) && master.get_digital(DIGITAL_L2) && clawMode == 1) {
//
// 	}
// }
//
// void automatic() {
//
// }
