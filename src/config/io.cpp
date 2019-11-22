#include "main.h"

pros::Controller master(CONTROLLER_MASTER);

// Sensors
pros::ADIPotentiometer rackPot(RACKPOT);
pros::ADIDigitalIn armLimit(ARMLIMIT);

pros::ADIGyro Gyro(GYRO);

// Ultrasonics
pros::ADIUltrasonic ultraL(ULTRALPING, ULTRALECHO);
pros::ADIUltrasonic ultraR(ULTRARPING, ULTRARECHO);
