#include "main.h"

#include "kari/control/chassis.h"
#include "kari/control/rack.h"
#include "kari/control/arm.h"

#include "kari/util/odometry.h"
#include "kari/util/misc.h"

#include "kari/displayController.h"
#include "kari/autonController.h"

void initialize() {
  okapi::Timer timer;
  double init = timer.millis().convert(millisecond);
  macro::print("Starting Init Routine");

  Autonomous Auton;
  macro::print("Auton Set!");

  // Class Initialization
  Odom odom;
  Chassis chassis(odom.getL(), odom.getR(), odom.getThetaDeg(), odom.getX(), odom.getY());
  Rack rack;
  Arm arm;
  Display Disp;

  // Roller Init
  io::RollerL.set_brake_mode(MOTOR_BRAKE_HOLD);
	io::RollerR.set_brake_mode(MOTOR_BRAKE_HOLD);

  // Sensor Init
  chassis.calibrateGyro();

  // Threads
  pros::Task odomController(odom.start, NULL, "Odometry Tracker");

  pros::Task baseController(chassis.start, NULL, "Chassis Controller");

  pros::Task rackController(rack.start, NULL, "Rack Controller");
  rack.setBrakeType(MOTOR_BRAKE_HOLD);

  pros::Task armController(arm.start, NULL, "Arm Controller");
  arm.setBrakeType(MOTOR_BRAKE_HOLD);
  arm.tarePos();

  pros::Task b_display(Disp.start, NULL, "Display Controller");
  b_display.set_priority(TASK_PRIORITY_MIN);

  pros::Task b_auton(Auton.start, NULL, "Auton Controller");
  b_display.set_priority(TASK_PRIORITY_MIN);

  Disp.addInfo("Left", 'i', odom.getL())
      .addInfo("Right", 'i', odom.getR())
      .addInfo("Rad Theta", 'd', odom.getThetaRad())
      .addInfo("Deg Theta", 'd', odom.getThetaDeg())
      .addInfo("X", 'd', odom.getX())
      .addInfo("Y", 'd', odom.getY());

  double end = timer.millis().convert(millisecond);
  std::cout << "Initialization Done! Took " << end - init << "ms." << std::endl;
  io::master.rumble(" .");
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
