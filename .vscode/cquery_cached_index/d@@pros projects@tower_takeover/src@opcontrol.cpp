#include "main.h"
#include "config.h"

int ctrlMode = 1; // 1 - Manual, 2 - Automatic
int clawMode = 1; // 1 - Open, 0 - Close

void automatic();
void manual();

void opcontrol() {
	while (true) {
		LF.move_velocity(master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_RIGHT_X) / 2);
		LB.move_velocity(master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_RIGHT_X) / 2);
		RF.move_velocity(master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_RIGHT_X) / 2);
		RB.move_velocity(master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_RIGHT_X) / 2);

		if((master.get_digital(DIGITAL_L1) || master.get_digital(DIGITAL_L2)) && ctrlMode == 2) ctrlMode = 1;

		if((master.get_digital(DIGITAL_R1) || master.get_digital(DIGITAL_R2)) && ctrlMode == 1) ctrlMode = 2;

		if(ctrlMode == 1) {
			manual();
		} else if(ctrlMode == 2) {
			automatic();
		}
	}
}

void manual() {
	if(master.get_digital(DIGITAL_L1) && master.get_digital(DIGITAL_L2) && clawMode == 0) {
		ClawL.move_absolute(0.1, 30);
		ClawR.move_absolute(0.1, 30);
	} else if(master.get_digital(DIGITAL_L1) && master.get_digital(DIGITAL_L2) && clawMode == 1) {

	}
}

void automatic() {

}
