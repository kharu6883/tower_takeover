#include "main.h"

#include "config/motor.h"

#include "control/displayController.h"
#include "control/autonController.h"
#include "control/asyncController.h"
using namespace Display;

ControlAsync Start;
pros::Task controlDrive(Start.run);
pros::Task armAsync(macroTask);

void initialize() {
  initAuton();

  controlDrive.suspend();
  armAsync.suspend();

  // Reset Motor Positions. Note - Rack has its own potentiometer
  Arm.tare_position();

  Rack.set_brake_mode(MOTOR_BRAKE_HOLD);
  Arm.set_brake_mode(MOTOR_BRAKE_HOLD);

  BrainDisplay brain;
  brain.main();

  RemoteDisplay remote;

  pros::Task updateDisplay(brain.update);
  updateDisplay.set_priority(TASK_PRIORITY_MIN);
  // pros::Task updateRemote(remote.update);

  wait(2000);
  brain.cleanup();
  std::cout << "Done!" << std::endl;
}

void disabled() {
  controlDrive.suspend();
  armAsync.suspend();
}

void competition_initialize() {

}
