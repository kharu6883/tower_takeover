#include "main.h"

pros::Controller master(CONTROLLER_MASTER);

pros::ADIDigitalIn selector(SELECTOR);

pros::ADIGyro Gyro(GYRO);

// Ultrasonics
pros::ADIUltrasonic ultraL(ULTRALPING, ULTRALECHO);
pros::ADIUltrasonic ultraR(ULTRARPING, ULTRARECHO);
