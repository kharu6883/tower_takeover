#include "chassis.h"

Chassis::Chassis(double kP_, double kD_)
: kP(kP_), kD(kD_),
LF(LFPORT, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS),
LB(LBPORT, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS),
RF(RFPORT, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS),
RB(RBPORT, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS) {
  usingThread = false;
}

Chassis::~Chassis() {
  reset();
}

Chassis& Chassis::withTolerance(double tolerance_) {
  return *this;
}

Chassis& Chassis::withSlop(double amp_, double offset_) {
  amp = amp_;
  amp = offset_;
  return *this;
}

Chassis& Chassis::runOnThread() {
  if(!isRunning) pros::Task ChassisController(start, NULL, "Chassis Controller");
  usingThread = true;
  return *this;
}

Chassis& Chassis::drive(double target_, int speed_, int rate_) {
  target = target_;
  speed = speed_;
  rate = rate_;
  return *this;
}

Chassis& Chassis::turn(double target_, int speed_, int rate_) {
  target = target_;
  speed = speed_;
  rate = rate_;
  return *this;
}

Chassis& Chassis::strafe(double target_, int speed_, int rate_) {
  target = target_;
  speed = speed_;
  rate = rate_;
  return *this;
}

Chassis& Chassis::align(double target_, double tolerance_) {
  target = target_;
  return *this;
}

void Chassis::waitUntilSettled() {
  while(!isSettled) pros::delay(20);
}

void Chassis::reset() {
  target = speed = rate = 0;
  current = last = error = derivative = output = slewOutput = 0;

  usingThread = false;
}

void Chassis::lock() {
  LF.set_brake_mode(MOTOR_BRAKE_HOLD);
  LB.set_brake_mode(MOTOR_BRAKE_HOLD);
  RF.set_brake_mode(MOTOR_BRAKE_HOLD);
  RB.set_brake_mode(MOTOR_BRAKE_HOLD);
}

void Chassis::unlock() {
  LF.set_brake_mode(MOTOR_BRAKE_COAST);
  LB.set_brake_mode(MOTOR_BRAKE_COAST);
  RF.set_brake_mode(MOTOR_BRAKE_COAST);
  RB.set_brake_mode(MOTOR_BRAKE_COAST);
}

void Chassis::run() {
  isRunning = true;

  while(isRunning) {

    pros::delay(20);
  }
}

void Chassis::start(void* ignore) {
  if(!isRunning) {
    pros::delay(500);
    Chassis *that = static_cast<Chassis*>(ignore);
    that -> run();
  }
}

void Chassis::stop() {
  reset();
  isRunning = false;
}

void Chassis::left(int speed) {
  LF.move_velocity(speed);
  LB.move_velocity(speed);
}

void Chassis::right(int speed) {
  RF.move_velocity(-speed);
  RB.move_velocity(-speed);
}

double Chassis::slop(int mode) {
  switch(mode) {
    case 1: return ((LF.get_position() + LB.get_position()) / 2) + ((RF.get_position() + RB.get_position()) / 2);
    default: return ((LF.get_position() + LB.get_position() / 2) - ((RF.get_position() + RB.get_position()) / 2) + offset) * amp; break;
  }
}
