#include "kari/control/rack.h"

pros::Motor RackMotor(16, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_ROTATIONS);
pros::ADIPotentiometer Pot(2);

bool Rack::isRunning = false,
Rack::isSettled = true,
Rack::isActive = false;

double Rack::kP = 0.08, Rack::tolerance = 5, Rack::target = 0;
int Rack::speed = 0, Rack::rate = 0;

int Rack::current = 0;
double Rack::error = 0, Rack::output = 0, Rack::slewOutput = 0;

Rack::Rack() { }

Rack::~Rack() { }

/*--------------------------------
    GETTERS & SETTERS
--------------------------------*/

Rack& Rack::withGain(double kP_) {
  kP = kP_;
  return *this;
}

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
  RackMotor.set_brake_mode(type_);
}

bool Rack::getState() {
  return isSettled;
}

int * Rack::getPot() {
  return &current;
}

/*--------------------------------
    TASK
--------------------------------*/

void Rack::start(void *ignore) {
  if(!isRunning) {
    pros::delay(500);
    Rack *that = static_cast<Rack*>(ignore);
    that -> run();
  }
}

void Rack::run() {
  isRunning = true;

  while(isRunning) {
    current = Pot.get_value();

    if(!pros::competition::is_autonomous()) goto end;

    if(isActive) {
      error = target - Pot.get_value();
      output = error * kP;

      if(output > 0) {
        if(output > slewOutput + rate) {
          slewOutput += rate;
        } else {
          slewOutput =  output;
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

      if(output > -tolerance && output < tolerance) {
        isSettled = true;
        withGain().withTol().reset();
        goto end;
      }

      move(slewOutput);
    }

    #ifdef DEBUG
    std::cout << "Rack Output: " << output << std::endl;
    #endif

    end:
    pros::delay(10);
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
  RackMotor.move(speed);
}
