#include "controller/rack.h"

bool Rack::isRunning = false,
Rack::isSettled = true,
Rack::isActive = false;

double Rack::error = 0, Rack::output = 0, Rack::slewOutput = 0;

Rack::Rack(double kP_) : kP(kP_),
Motor(RACK, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_ROTATIONS),
Pot(RACKPOT) { }

Rack::~Rack() { }

/*--------------------------------
    GETTERS & SETTERS
--------------------------------*/

Rack& Rack::withTol(double tolerance_) {
  tolerance = tolerance_;
  return *this;
}

Rack& Rack::move(double target_, int speed_, int rate_) {
  target = target_;
  speed = speed_;
  rate = rate_;
  isSettled = false;
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

void Rack::setBrakeType(pros::motor_brake_mode_e_t type_) {
  Motor.set_brake_mode(type_);
}

bool Rack::getState() {
  return isSettled;
}

int Rack::getPot() {
  return Pot.get_value();
}

/*--------------------------------
    TASK
--------------------------------*/

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
        withTol().reset();
        isSettled = true;
        goto end;
      }

      move(slewOutput);
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

/*--------------------------------
    MOVEMENT
--------------------------------*/

void Rack::move(int speed) {
  Motor.move(speed);
}
