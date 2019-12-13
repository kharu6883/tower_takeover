#include "main.h"

#include "config/io.h"
#include "config/motor.h"

#include "control/displayController.h"
#include "control/autonController.h"
#include "control/asyncController.h"
#include "control/macro.h"
#include "control/path.h"
using namespace Display;

static ControlAsync Control;
pros::Task armAsync(macroTask, NULL, "Opcontrol Arm Task");

void initialize() {
  print("Starting Init Routine");

  Autonomous Auton;
  print("Auton Set!");

  armAsync.suspend();

  // Motor Initialization
  Arm.tare_position();

<<<<<<< HEAD
  Rack.set_brake_mode(MOTOR_BRAKE_HOLD);
  Arm.set_brake_mode(MOTOR_BRAKE_HOLD);

  RollerL.set_brake_mode(MOTOR_BRAKE_HOLD);
	RollerR.set_brake_mode(MOTOR_BRAKE_HOLD);
  print("Motors Initialized!");
=======
  Gyro.reset();

  // Motor Settings
  Rack.set_brake_mode(MOTOR_BRAKE_HOLD);
  Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
  print("Motor Set!");
>>>>>>> parent of 9e3da2f... calgary comp

  // Threads
  pros::Task asyncController(Control.run, NULL, "Async Controller");
  Control.pause();
  print("Task Controller Initialized!");

  BrainDisplay Brain;
  Brain.main();
  pros::Task b_display(Brain.run, NULL, "Brain Display");
  b_display.set_priority(TASK_PRIORITY_MIN);
  print("Display Initialized!");

  RemoteDisplay Remote;
  pros::Task r_display(Remote.run, NULL, "Remote Display");
  r_display.set_priority(TASK_PRIORITY_MIN);
  print("Remote Initialized!");

  // Path path;
  // pros::Task pathMaker(path.start, NULL, "Path Maker");
  // pathMaker.set_priority(TASK_PRIORITY_MIN);
  // print("Pathmaker Initialized!");

  std::cout << "Initialization Done!" << std::endl;
}

void disabled() {
  Control.pause();
  armAsync.suspend();
}

void competition_initialize() { }
