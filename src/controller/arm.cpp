#include "controller/arm.h"
#include "controller/misc.h"

pros::Motor ArmMotor(ARM, MOTOR_GEARSET_36, 0, MOTOR_ENCODER_ROTATIONS);
pros::ADIDigitalIn Limit(ARMLIMIT);

bool Arm::isRunning = false,
Arm::isSettled = true,
Arm::macroFin = true,
Arm::reached = false;

int Arm::mode = 0,
Arm::nextCmd = 0;

double Arm::target = 0, Arm::tolerance = 3;
int Arm::speed = 0, Arm::rate = 9;

double Arm::kP = 250;

double Arm::error = 0, Arm::output = 0, Arm::slewOutput = 0;

Arm::Arm() { }

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
  mode = 11;
  return *this;
}

Arm& Arm::tower(int tower) {
  target = ARM_BOTTOM;
  speed = 127;
  rate = 9;

  if(tower == 1 || tower == 2) {
    macroFin = false;
    nextCmd = tower;
    mode = 11;
  } else {
    nextCmd = 0;
    mode = tower;
  }

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

void Arm::reset() {
  error = output = slewOutput = 0;
}

void Arm::tarePos() {
  ArmMotor.tare_position();
}

void Arm::setBrakeType(pros::motor_brake_mode_e_t type_) {
  ArmMotor.set_brake_mode(type_);
}

int Arm::getMode() {
  return mode;
}

bool Arm::getState() {
  return isSettled;
}

bool Arm::getMacroState() {
  return macroFin;
}

bool Arm::getLimit() {
  return Limit.get_value();
}

void Arm::run() {
  isRunning = true;
  double rollerPrePrime = 100, rollerWait = 150, rollerRot = -0.8, rollerSpeed = 150, rollerWaitPrime = 100;

  while(isRunning) {
    switch(mode) {

      // Low Tower
      case 1: {
        io::roller(rollerPrePrime);
        pros::delay(rollerWait);
        io::roller(rollerRot, rollerSpeed);
        pros::delay(rollerWaitPrime);
        macroFin = true;

        target = ARM_LOW_TOWER;
        nextCmd = 0;
        mode = 11;
        break;
      }

      // Mid Tower
      case 2: {
        io::roller(rollerPrePrime);
        pros::delay(rollerWait);
        io::roller(rollerRot, rollerSpeed);
        pros::delay(rollerWaitPrime);
        macroFin = true;

        target = ARM_MID_TOWER;
        nextCmd = 0;
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

      // 2 Cube Height
      case 7: {
        target = ARM_TWO_CUBE;
        mode = 11;
        break;
      }

      // Reset
      case 10: {
        move(-100);
        if(Limit.get_value()) {
          ArmMotor.tare_position();
          move(0);
          mode = 11;
        }

        break;
      }

      // MovePos
      case 11: {
        output = (target - ArmMotor.get_position()) * kP;

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

        if((output > -tolerance && output < tolerance) || (target == ARM_BOTTOM && Limit.get_value())) {
          if(nextCmd != 0) {
            mode = nextCmd;
            break;
          } else { withTol(); nextCmd = 0; mode = 0; break; }
        }

        if(slewOutput > speed) slewOutput = speed;
        if(slewOutput < -speed) slewOutput = -speed;

        move(slewOutput);
        break;
      }

      default: {
        ArmMotor.set_brake_mode(MOTOR_BRAKE_HOLD);
        break;
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

void Arm::move(int speed) {
  ArmMotor.move(speed);
}
