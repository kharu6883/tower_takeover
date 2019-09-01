#include "main.h"

#include "config/io.h"
#include "config/motor.h"
#include "config/vision.h"

#include "control/displayController.h"
#include "control/autonController.h"
#include "control/asyncController.h"
#include "control/macro.h"
using namespace Display;

ControlAsync Control;
pros::Task armAsync(macroTask);

void initialize() {
  initAuton();
  initVision();

  armAsync.suspend();

  // Reset Motor Positions. Note - Rack has its own potentiometer
  Arm.tare_position();

  Rack.set_brake_mode(MOTOR_BRAKE_HOLD);
  Arm.set_brake_mode(MOTOR_BRAKE_HOLD);

  BrainDisplay Brain;
  Brain.main();
  pros::Task display(Brain.run);

  wait(2000);
  Brain.cleanup();
  std::cout << "Done!" << std::endl;
}

void disabled() {
  Control.stop();
  armAsync.suspend();
}

void competition_initialize() {

}
