#include "main.h"

#include "controller/chassis.h"
#include "controller/rack.h"
#include "controller/arm.h"
#include "controller/misc.h"

#include "controller/displayController.h"
#include "controller/autonController.h"

void initialize() {
  macro::print("Starting Init Routine");

  Autonomous Auton;
  macro::print("Auton Set!");

  // Motor Initialization
  Rack rack;
  Arm arm;

  arm.tarePos();

  rack.setBrakeType(MOTOR_BRAKE_HOLD);
  arm.setBrakeType(MOTOR_BRAKE_HOLD);

  io::RollerL.set_brake_mode(MOTOR_BRAKE_HOLD);
	io::RollerR.set_brake_mode(MOTOR_BRAKE_HOLD);
  macro::print("Motors Initialized!");

  // Threads
  Chassis chassis(0.6, 0.6);
  pros::Task baseController(chassis.start, NULL, "Chassis Controller");
  macro::print("Chassis Initialized!");

  pros::Task rackController(rack.start, NULL, "Rack Controller");
  macro::print("Rack Initialized!");

  pros::Task armController(arm.start, NULL, "Rack Controller");
  macro::print("Arm Initialized!");

  Display Disp;
  pros::Task b_display(Disp.start, NULL, "Brain Display");
  b_display.set_priority(TASK_PRIORITY_MIN);
  macro::print("Display Initialized!");

  // Path Pathmaker;
  // pros::Task pathMaker(Pathmaker.start, NULL, "PathMaker");
  // pathMaker.set_priority(TASK_PRIORITY_MIN);
  // print("PathMaker Initialized!");

  pros::Task autonSelect(Auton.start, NULL, "Auton Selector");
  autonSelect.set_priority(TASK_PRIORITY_MIN);
  macro::print("Auton Selector Initialized!");

  std::cout << "Initialization Done!" << std::endl;
}

void disabled() { }

void competition_initialize() { }
