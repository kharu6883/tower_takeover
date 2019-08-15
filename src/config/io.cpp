#include "main.h"

pros::Controller master(pros::E_CONTROLLER_PARTNER);  //for comp
//pros::Controller master(pros::E_CONTROLLER_MASTER); //for downloading
// Sensors
pros::ADIPotentiometer rackPot(RACKPOT);
pros::ADILineSensor rollerline(ROLLERLINE);

// Ultrasonics
pros::ADIUltrasonic ultraL(ULTRALPING, ULTRALECHO);
pros::ADIUltrasonic ultraR(ULTRARPING, ULTRARECHO);
