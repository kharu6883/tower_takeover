#include "controller/rack.h"

Rack::Rack(double kP_) : kP(kP_),
Motor(RACK, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_ROTATIONS),
Pot(RACKPOT) { }

Rack::~Rack() { }

Rack& Rack::withTolerance(double tolerance_) {
  tolerance = tolerance_;
  return *this;
}

Rack& Rack::move(double target_, int speed_, int rate_) {
  target = target_;
  speed = speed_;
  rate = rate_;
  isActive = true;
  return *this;
}

void Rack::waitUntilSettled() {
  while(!isSettled) pros::delay(20);
}

void Rack::reset() {
  error = output = slewOutput = 0;
  isActive = false;
}

bool Rack::getState() {
  return isSettled;
}

int Rack::getPot() {
  return Pot.get_value();
}

void Rack::run() {
  isRunning = true;

  while(isRunning) {

    if(isActive) {
      error = target - Pot.get_value();
      output = error * kP;

      if(output > 0) {
        if(output > slewOutput + rate) {
          slewOutput += rate;
        } else {
          slewOutput = output;
        }
      }

      if(output < 0) {
        if(output < slewOutput - rate) {
          slewOutput -= rate;
        } else {
          slewOutput = output;
        }
      }

      if(slewOutput > speed) slewOutput = speed;

      if(-tolerance < error < tolerance) {
        reset();
        withTolerance();
        goto end;
      }

      rack(slewOutput);
    }

    end:
    pros::delay(20);
  }
}

void Rack::start(void *ignore) {
  if(!isRunning) {
    pros::delay(500);
    Rack *that = static_cast<Rack*>(ignore);
    that -> run();
  }
}

void Rack::stop() {
  isRunning = false;
  reset();
}
