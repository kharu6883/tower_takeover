#include "main.h"

#include "controller/chassis.h"
#include "controller/rack.h"
#include "controller/arm.h"
// #include "controller/path.h"
#include "controller/misc.h"

#include "controller/displayController.h"
#include "controller/autonController.h"

void initialize() {
  okapi::Timer timer;
  double init = timer.millis().convert(millisecond);
  macro::print("Starting Init Routine");

  Autonomous Auton;

  // Roller Init
  io::RollerL.set_brake_mode(MOTOR_BRAKE_HOLD);
	io::RollerR.set_brake_mode(MOTOR_BRAKE_HOLD);

  // Threads
  Chassis chassis;
  pros::Task baseController(chassis.start, NULL, "Chassis Controller");

  Rack rack;
  pros::Task rackController(rack.start, NULL, "Rack Controller");
  rack.setBrakeType(MOTOR_BRAKE_HOLD);

  Arm arm;
  pros::Task armController(arm.start, NULL, "Arm Controller");
  arm.setBrakeType(MOTOR_BRAKE_HOLD);
  arm.tarePos();

  Display Disp;
  pros::Task b_display(Disp.start, NULL, "Brain Display");
  b_display.set_priority(TASK_PRIORITY_MIN);

  // Path path;
  // pros::Task pather(path.start, NULL, "PathMaker");
  // pather.set_priority(TASK_PRIORITY_MIN);

  double end = timer.millis().convert(millisecond);
  std::cout << "Initialization Done! Took " << end - init << "ms." << std::endl;
}

void disabled() {
  Chassis chassis;
  Rack rack;
  Arm arm;

  chassis.withConst().withTol().withSlop().reset();
  chassis.setMode(IDLE);
  chassis.clearArr();
  rack.withTol().reset();
  arm.withTol().reset();
}

void competition_initialize() { }
