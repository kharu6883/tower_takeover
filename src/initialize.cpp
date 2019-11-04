#include "main.h"

#include "config/io.h"
#include "config/motor.h"

#include "control/displayController.h"
#include "control/autonController.h"
#include "control/asyncController.h"
#include "control/macro.h"
#include "control/path.h"

static ControlAsync Control;
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
  pros::Task asyncController(Control.run, NULL, "Async Controller");
  Control.pause();
  print("Task Controller Initialized!");

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

void disabled() {
  Control.pause();
  armAsync.suspend();
}

void competition_initialize() { }
