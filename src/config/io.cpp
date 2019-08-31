#include "main.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);

// Sensors
pros::ADIPotentiometer rackPot(RACKPOT);
pros::ADIPotentiometer armPot(ARMPOT);
pros::ADIDigitalIn armLimit(ARMLIMIT);

// Ultrasonics
pros::ADIUltrasonic ultraL(ULTRALPING, ULTRALECHO);
pros::ADIUltrasonic ultraR(ULTRARPING, ULTRARECHO);
