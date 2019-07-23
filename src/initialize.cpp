#include "main.h"
#include "config.h"
using namespace Display;

void initialize() {
  initAuton();

  Rack.tare_position();
  Arm.tare_position();

  Rack.set_brake_mode(MOTOR_BRAKE_HOLD);
  Arm.set_brake_mode(MOTOR_BRAKE_HOLD);

  Display::BrainDisplay brain;
  brain.main();

  Display::RemoteDisplay remote;

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
