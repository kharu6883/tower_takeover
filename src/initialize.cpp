#include "main.h"

#include "config/io.h"
#include "config/motor.h"

#include "control/displayController.h"
#include "control/autonController.h"
#include "control/asyncController.h"
#include "control/macro.h"
using namespace Display;

static ControlAsync Control;
pros::Task armAsync(macroTask);

void initialize() {
  Autonomous Auton;
  print("Auton Set");

  pros::Task asyncController(Control.run);
  Control.pause();
  print("Task Controller Done");

  armAsync.suspend();

  // Reset Motor Positions. Note - Rack has its own potentiometer
  Arm.tare_position();

  Rack.set_brake_mode(MOTOR_BRAKE_HOLD);
  Arm.set_brake_mode(MOTOR_BRAKE_HOLD);
  print("Motor Set");

  BrainDisplay Brain;
  Brain.main();
  pros::Task b_display(Brain.run);
  b_display.set_priority(TASK_PRIORITY_MIN);
  print("Display Done");

  RemoteDisplay Remote;
  pros::Task r_display(Remote.run);
  r_display.set_priority(TASK_PRIORITY_MIN);
  print("Remote Done");

  wait(2000);
  Brain.cleanup();
  std::cout << "Done!" << std::endl;
}

void disabled() {
  Control.pause();
  armAsync.suspend();
}

void competition_initialize() {

}
