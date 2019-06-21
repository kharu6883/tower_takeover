#include "main.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::Motor LF(LFPORT, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS); // 9 11
pros::Motor LB(LBPORT, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS); // 20 1
pros::Motor RF(RFPORT, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_ROTATIONS); // 10 20
pros::Motor RB(RBPORT, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_ROTATIONS); // 19 10

pros::Motor LiftL(2, pros::E_MOTOR_GEARSET_18);
pros::Motor LiftR(9, pros::E_MOTOR_GEARSET_18);

pros::Motor ClawL(5);
pros::Motor ClawR(7);

pros::ADIPotentiometer Arm(1);
