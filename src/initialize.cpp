#include "main.h"
#include "control/config.h"
using namespace Display;

void initialize() {
  initAuton();
  void* boi;

  // Start Asynchronous Driver
  ControlAsync Start;
  pros::Task controlDrive(Start.run);

  // Reset Motor Positions. Note - Rack has its own potentiometer
  Arm.tare_position();

  Rack.set_brake_mode(MOTOR_BRAKE_HOLD);
  Arm.set_brake_mode(MOTOR_BRAKE_HOLD);

  BrainDisplay brain;
  brain.main();

  RemoteDisplay remote;

  pros::Task updateDisplay(brain.update);
  pros::Task updateRemote(remote.update);

  wait(2000);
  brain.cleanup();
  std::cout << "Done!" << std::endl;
}

void disabled() {

}

void competition_initialize() {

}
