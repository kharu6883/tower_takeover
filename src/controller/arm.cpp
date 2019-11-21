#include "controller/arm.h"
#include "controller/misc.h"

bool Arm::isRunning = false,
Arm::isSettled = true,
Arm::keepPos = true;

int Arm::mode = 0,
Arm::nextCmd = 0;

double Arm::kP = 300;

double Arm::error = 0, Arm::output = 0, Arm::slewOutput = 0;

Arm::Arm() : Motor(ARM, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS),
Limit(ARMLIMIT) { }

Arm::~Arm() { }

Arm& Arm::withTol(double tolerance_) {
  tolerance = tolerance_;
  return *this;
}

Arm& Arm::move(double target_, int speed_, int rate_) {
  target = target_;
  speed = speed_;
  rate = rate_;
  nextCmd = 0;
  mode = 0;
  return *this;
}

Arm& Arm::tower(int tower) {
  target = ARM_BOTTOM;
  speed = 127;
  rate = 9;

  if(tower == 1 || tower == 2) {
    nextCmd = tower;
    mode = 11;
  } else mode = tower;

  return *this;
}

Arm& Arm::zero() {
  target = ARM_BOTTOM;
  reached = Limit.get_value();
  nextCmd = 0;
  mode = 10;
  return *this;
}

void Arm::waitUntilSettled() {
  while(!isSettled) pros::delay(20);
}

void Arm::keepPosIfSettled() {
  keepPos = true;
}

void Arm::reset() {
  error = output = slewOutput = 0;
}

void Arm::tarePos() {
  Motor.tare_position();
}

void Arm::setBrakeType(pros::motor_brake_mode_e_t type_) {
  Motor.set_brake_mode(type_);
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
  double rollerRot = -0.8, rollerSpeed = 150, rollerWaitPrime = 100, rollerWait = 200, rollerPrePrime = 100;

  while(isRunning) {
    if(!io::master.is_connected()) { mode = 10; goto end; }

    switch(mode) {

      // Low Tower
      case 1: {
        io::roller(rollerPrePrime);
        pros::delay(rollerWait);
        io::roller(rollerRot, rollerSpeed);
        pros::delay(rollerWaitPrime);

        target = ARM_LOW_TOWER;
        mode = 11;
        break;
      }

      // Mid Tower
      case 2: {
        io::roller(rollerPrePrime);
        pros::delay(rollerWait);
        io::roller(rollerRot, rollerSpeed);
        pros::delay(rollerWaitPrime);

        target = ARM_MID_TOWER;
        mode = 11;
        break;
      }

      // Low Tower Manual
      case 3: {
        target = ARM_LOW_TOWER_MANUAL;
        mode = 11;
        break;
      }

      // Low Tower Descore
      case 4: {
        target = ARM_LOW_TOWER_DESCORE;
        mode = 11;
        break;
      }

      // Mid Tower Descore
      case 5: {
        target = ARM_MID_TOWER_DESCORE;
        mode = 11;
        break;
      }

      // Low Tower... again?
      case 6: {
        io::roller(rollerPrePrime);
        pros::delay(rollerWait);
        io::roller(rollerRot, rollerSpeed);
        pros::delay(rollerWaitPrime);

        target = ARM_LOW_TOWER;
        mode = 11;
        break;
      }

      // Reset
      case 10: {
        move(-127);
        if(Limit.get_new_press()) {
          Motor.tare_position();
          move(0);
          keepPos = true;
          target = ARM_BOTTOM;
          mode = 11;
          goto end;
        } else if(reached) {
          keepPos = true;
          target = ARM_BOTTOM;
          mode = 11;
          goto end;
        }

        break;
      }

      // MovePos
      case 11: {
        output = (target - Motor.get_position()) * kP;

        if(output > 0) {
          if(slewOutput > output + rate) {
            slewOutput += rate;
          } else {
            slewOutput = output;
          }
        }

        if(output < 0) {
          if(slewOutput < output - rate) {
            slewOutput -= rate;
          } else {
            slewOutput = output;
          }
        }

        if(-tolerance < output < tolerance || Limit.get_value()) {
          if(nextCmd != 0) {
            mode = nextCmd;
            goto end;
          } else if(!keepPos) { withTol(); nextCmd = 0; mode = 0; goto end; }
        }

        if(slewOutput > speed) slewOutput = speed;
        if(slewOutput < -speed) slewOutput = -speed;

        move(slewOutput);
        break;
      }

      default: {
        break;
      }
    }

    end:
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

void Arm::move(int speed) {
  Motor.move(speed);
}
