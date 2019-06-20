#include "main.h"

Controller master(E_CONTROLLER_MASTER);

Motor LF(9, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS); // 9 11
Motor LB(20, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS); // 20 1
Motor RF(10, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_ROTATIONS); // 10 20
Motor RB(19, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_ROTATIONS); // 19 10

Motor LiftL(2, E_MOTOR_GEARSET_18);
Motor LiftR(9, E_MOTOR_GEARSET_18);

Motor ClawL(5);
Motor ClawR(7);

pros::ADIAnalogIn Arm(1);
pros::ADIEncoder OL(1, 2, false);
pros::ADIEncoder OR(3, 4, false);
