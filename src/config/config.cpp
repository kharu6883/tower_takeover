#include "main.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::Motor LF(9, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS); // 9 11
pros::Motor LB(20, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS); // 20 1
pros::Motor RF(10, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_ROTATIONS); // 10 20
pros::Motor RB(19, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_ROTATIONS); // 19 10

pros::Motor LiftL(2, pros::E_MOTOR_GEARSET_18);
pros::Motor LiftR(9, pros::E_MOTOR_GEARSET_18);

pros::Motor ClawL(5);
pros::Motor ClawR(7);

pros::ADIAnalogIn Arm(1);
pros::ADIEncoder OL(1, 2, false);
pros::ADIEncoder OR(3, 4, false);
