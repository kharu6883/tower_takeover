#include "controller/arm.h"

Arm::Arm(double kP_) : kP(kP_),
Motor(ARM, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS),
Limit(ARMLIMIT) { }

Arm::~Arm() {

}

Arm& Arm::withTolerance(double tolerance_) {
  tolerance = tolerance_;
  return *this;
}

Arm& Arm::move(double target_, int speed_, int rate_) {
  target = target_;
  speed = speed_;
  rate = rate_;
  mode = MOVING;
  return *this;
}

void Arm::waitUntilSettled() {
  while(!isSettled) pros::delay(20);
}

void Arm::reset() {
  error = output = slewOutput = 0;

}

bool Arm::getState() {
  return isSettled;
}

int Arm::getMode() {
  return mode;
}

bool Arm::getLimit() {
  return Limit.get_value();
}

void Arm::run() {
  isRunning = true;

  while(true) {
    if(pros::competition::is_autonomous()) {

    } else {
      
    }

    pros::delay(20);
  }
}

void Arm::start(void *ignore) {
  if(!isRunning) {
    pros::delay(500);
    Arm *that = static_cast<Arm*>(ignore);
    that -> run();
  }
}

void Arm::stop() {
  isRunning = false;
  reset();
}
