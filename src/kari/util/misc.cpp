#include "main.h"
#include "kari/util/misc.h"

namespace io {
  pros::Controller master(CONTROLLER_MASTER);

  pros::Motor RollerL(5, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS); // Left Roller
  pros::Motor RollerR(10, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_ROTATIONS); // Right Roller

  void roller(int speed) {
    RollerL.move(speed);
    pros::delay(50);
    RollerR.move(speed);
    pros::delay(50);
    RollerL.move(speed);
    pros::delay(50);
    RollerR.move(speed);
  }

  void driveRoller(int speed) {
    RollerL.move(speed);
    RollerR.move(speed);
  }

  void roller(double rot, int speed) {
    RollerL.move_relative(rot, speed);
    pros::delay(50);
    RollerR.move_relative(rot, speed);
  }
}

namespace macro {
  Slew::Slew(double accel_) : accel(accel_), decel(0) {
    noDecel = true;
  }

  Slew::Slew(double accel_, double decel_) : accel(accel_), decel(decel_) {
    noDecel = false;
  }

  Slew::Slew(double accel_, double decel_, bool reversible_) : accel(accel_), decel(decel_), isReversible(reversible_) {
    noDecel = false;
  }

  Slew& Slew::withLimit(double input) {

    isLimited = true;
    limit = input;

    return *this;
  }

  double Slew::calculate(double input) {
    if(!noDecel) {

      if(!isReversible) {
        if(input > output + accel) {
          output += accel;
        } else if(input < output - decel) {
          output -= decel;
        } else {
          output = input;
        }
      } else {
        if(input > 0) {
          if(input > output + accel) {
            output += accel;
          } else if(input < output - decel) {
            output -= decel;
          } else {
            output = input;
          }
        }

        if(input < 0) {
          if(input < output - accel) {
            output -= accel;
          } else if(input > output + decel) {
            output += decel;
          } else {
            output = input;
          }
        }

        if(input == 0) {
          if(input < output - decel) {
            output -= decel;
          } else if(input > output + decel) {
            output += decel;
          } else {
            output = input;
          }
        }
      }


      if(isLimited) {
        if(limit > 0 && output > limit) {
          output = limit;
        }

        if(limit < 0 && output < limit) {
          output = limit;
        }
      }

    } else {
      if(input > 0) {
        if(input > output + accel) output += accel;
        else output = input;
      }

      if(input < 0) {
        if(input < output - accel) output -= accel;
        else output = input;
      }
    }

    return output;
  }

  void Slew::setOutput(double output_) {
    output = output_;
  }

  double Slew::getOutput() {
    return output;
  }

  void Slew::reset() {
    input = output = 0;
  }

  PID::PID(double kP_) : kP(kP_), kD(0) { }

  PID::PID(double kP_, double kD_) : kP(kP_), kD(kD_) { }

  PID& PID::withGain(double kP_) {
    kP = kP_;
    return *this;
  }

  PID& PID::withGain(double kP_, double kD_) {
    kP = kP_;
    kD = kD_;
    return *this;
  }

  double PID::calculate(double target, double input) {

    error = target - input;

    output = ( error * kP ) + ( error - last ) * kD;

    last = error;

    return output;
  }

  double PID::getError() {
    return error;
  }

  double PID::getOutput() {
    return output;
  }

  void wait(int ms) {
    pros::delay(ms);
  }

  void print(const char * text) {
    std::cout << text << std::endl;
  }
}
