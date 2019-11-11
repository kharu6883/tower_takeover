#include "controller/arm.h"

Arm::Arm(double kP_) : kP(kP_),
Motor(ARM, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS),
Limit(ARMLIMIT) { }

Arm::~Arm() { }

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

Arm& Arm::tower(int tower) {
  const double kP = 210;

  double armTarget;

  Arm.set_current_limit(7000);

  if(tower == 2) { // Mid Tower
    while(true) {
      armTarget = pTerm(ARM_BOTTOM, Arm.get_position(), kP + 400);
      arm(armTarget);

      if(isSettled(armTarget, tolerance) || armLimit.get_value()) { arm(0); break; }
      wait(20);
    }

    roller(rollerpreprime);
    wait(rollerWait);
    roller(rollerRot, rollerSpeed);
    wait(rollerwaitprime);
    while(true) {
      armTarget = pTerm(ARM_MID_TOWER, Arm.get_position(), kP);
      arm(armTarget);

      if(isSettled(armTarget, tolerance)) { arm(0); break; }
      wait(20);
    }
  } else if(tower == 3) {
    while(true) {
      armTarget = pTerm(ARM_LOW_TOWER_MANUAL, Arm.get_position(), kP + 10);
      arm(armTarget);

      if(isSettled(armTarget, tolerance)) { arm(0); break; }
      wait(20);
    }
  } else if(tower == 4) {
    while(true) {
      armTarget = pTerm(ARM_LOW_TOWER_DESCORE, Arm.get_position(), kP + 10);
      arm(armTarget);

      if(isSettled(armTarget, tolerance)) { arm(0); break; }
      wait(20);
    }
  } else if(tower == 5) {
    while(true) {
      armTarget = pTerm(ARM_MID_TOWER_DESCORE, Arm.get_position(), kP + 10);
      arm(armTarget);

      if(isSettled(armTarget, tolerance)) { arm(0); break; }
      wait(20);
    }
  }

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
  double rollerRot = -0.8, rollerSpeed = 150, rollerwaitprime=100, rollerWait = 200, rollerpreprime = 100;

  while(isRunning) {
    switch(mode) {
      case 1: {
        while(true) {
          output = (ARM_BOTTOM * Motor.get_position()) * kP;
          arm(output);

          if( || Limit.get_value()) { arm(0); break; }
          wait(20);
        }

        roller(rollerpreprime);
        wait(rollerWait);
        roller(rollerRot, rollerSpeed);
        wait(rollerwaitprime);
        while(true) {
          armTarget = pTerm(ARM_LOW_TOWER, Arm.get_position(), kP);
          arm(armTarget);

          if(isSettled(armTarget, tolerance)) { arm(0); break; }
          wait(20);
        }
      }
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
