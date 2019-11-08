#include "main.h"

#include "config/io.h"
#include "config/motor.h"

#include "controller/chassis.h"
#include "controller/rack.h"
#include "controller/misc.h"

#include "controller/displayController.h"
#include "controller/autonController.h"
#include "controller/path.h"


pros::Task armAsync(macroTask, NULL, "Opcontrol Arm Task");

void initialize() {
  print("Starting Init Routine");

  Autonomous Auton;
  print("Auton Set!");

  armAsync.suspend();

  // Motor Initialization
  Arm.tare_position();

  Rack.set_brake_mode(MOTOR_BRAKE_HOLD);
  Arm.set_brake_mode(MOTOR_BRAKE_HOLD);

  RollerL.set_brake_mode(MOTOR_BRAKE_HOLD);
	RollerR.set_brake_mode(MOTOR_BRAKE_HOLD);
  print("Motors Initialized!");

  // Threads
  Chassis chassis(0.6, 0.6);
  pros::Task baseController(chassis.start, NULL, "Chassis Controller");
  print("Chassis Initialized!");

  class Rack rack;
  pros::Task rackController(rack.start, NULL, "Rack Controller");
  print("Rack Initialized!");

  Display Disp;
  pros::Task b_display(Disp.start, NULL, "Brain Display");
  b_display.set_priority(TASK_PRIORITY_MIN);
  print("Display Initialized!");

  Path Pathmaker;
  pros::Task pathMaker(Pathmaker.start, NULL, "PathMaker");
  pathMaker.set_priority(TASK_PRIORITY_MIN);
  print("PathMaker Initialized!");

  pros::Task autonSelect(Auton.start, NULL, "Auton Selector");
  autonSelect.set_priority(TASK_PRIORITY_MIN);
  print("Auton Selector Initialized!");

  std::cout << "Initialization Done!" << std::endl;
}

void disabled() { }

void competition_initialize() { }
