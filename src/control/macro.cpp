#include "main.h"

#include "config/motor.h"
#include "config/io.h"

#include "control/macro.h"
#include "control/drive.h"

void rack(int speed) {
  Rack.move_velocity(speed);
}

void arm(int speed) {
  Arm.move_velocity(speed);
}

void roller(int speed) {
  RollerL.move_velocity(speed);
  wait(50);
  RollerR.move_velocity(speed);
}

void roller(double rot, int speed) {
  RollerL.move_relative(rot, speed);
  wait(20);
  RollerR.move_relative(rot, speed);
}



void rack(double target, int speed, double rate) {
  const double kP = 100;

  double tolerance = 3;

  double slewOutput;

  while(target > rackPot.get_value()) { // Goin' up
    double desired = pTerm(target, rackPot.get_value(), kP);

    if(desired > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = desired;
    }

    if(slewOutput > speed) slewOutput = speed;

    if(isSettled(desired, tolerance)) break;

    rack(slewOutput);

    wait(20);
  }

  while(target < rackPot.get_value()) { // Goin' down
    double desired = pTerm(target, rackPot.get_value(), kP);

    if(abs(desired) > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = abs(desired);
    }

    if(slewOutput > speed) slewOutput = speed;

    if(isSettled(desired, tolerance)) break;

    rack(-slewOutput);

    wait(20);
  }

  rack(0);
}

void arm(double target, int speed, double rate) {
  const double kP = 300;

  double tolerance = 2;

  double slewOutput;

  while(target > Arm.get_position()) { // Goin' up
    double desired = pTerm(target, Arm.get_position(), kP);

    if(desired > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = desired;
    }

    if(slewOutput > speed) slewOutput = speed;

    if(isSettled(desired, tolerance)) break;

    arm(slewOutput);

    wait(20);
  }

  while(target < Arm.get_position()) { // Goin' down
    double desired = pTerm(target, Arm.get_position(), kP);

    if(abs(desired) > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = abs(desired);
    }

    if(slewOutput > speed) slewOutput = speed;

    if(isSettled(desired, tolerance)) break;

    arm(-slewOutput);

    wait(20);
  }

  arm(0);
}

void tower(int tower) {

  const double kP = 210;
  double rollerRot = -0.6, rollerSpeed = 50,rollerwaitprime=20, rollerWait = 150, rollerpreprime = 100;
  double armTarget, tolerance = 6;

  #if DEBUG
  std::cout << Arm.get_current_limit() << std::endl;
  #endif

  Arm.set_current_limit(7000);

  if(tower == 1) { // Bottom Tower
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
      armTarget = pTerm(ARM_LOW_TOWER, Arm.get_position(), kP);
      arm(armTarget);

      if(isSettled(armTarget, tolerance)) { arm(0); break; }
      wait(20);
    }
  } else if(tower == 2) { // Mid Tower
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
  } else if(tower == 6) {
    roller(rollerpreprime);
    wait(rollerWait);
    roller(rollerRot, rollerSpeed);
    wait(rollerwaitprime);
    while(true) {
      armTarget = pTerm(ARM_LOW_TOWER, Arm.get_position(), kP + 10);
      arm(armTarget);

      if(isSettled(armTarget, tolerance)) { arm(0); break; }
      wait(20);
    }
  }
}

void tower(int tower, double tolerance) {

  const double kP = 210;
  double rollerRot = -0.8, rollerSpeed = 150, rollerwaitprime=100, rollerWait = 200, rollerpreprime = 100;
  double armTarget;

  #if DEBUG
  std::cout << Arm.get_current_limit() << std::endl;
  #endif

  Arm.set_current_limit(7000);

  if(tower == 1) { // Bottom Tower
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
      armTarget = pTerm(ARM_LOW_TOWER, Arm.get_position(), kP);
      arm(armTarget);

      if(isSettled(armTarget, tolerance)) { arm(0); break; }
      wait(20);
    }
  } else if(tower == 2) { // Mid Tower
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
}

void armReset() {
  Arm.set_current_limit(10000);
  arm(-200);

  bool isDown = armLimit.get_value();

  while(true) {
    if(armLimit.get_new_press()) {
      arm(0);
      Arm.tare_position();
      break;
    } else if(isDown) break;

    pros::delay(20);
  }

  arm(0);
  setReset(false);
}

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

PID& PID::withConst(double kP_) {
  kP = kP_;
  return *this;
}

PID& PID::withConst(double kP_, double kD_) {
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



double pTerm(double target, double sensor, double kP) {
  return(target - sensor) * kP;
}

double dTerm(double now, double last) {
  return (now - last);
}

bool isSettled(double error, double tolerance) {
  bool settled = false;

  if(error > -tolerance && error < tolerance) settled = true;

  return settled;
}

double slop(double amp_) {
  const double amp = 8;

  double deltaL = ( LF.get_position() + LB.get_position() ) / 2;
  double deltaR = ( RF.get_position() + RB.get_position() ) / 2;

  return ( deltaL - deltaR ) / (amp + amp_);
}

double slop(int mode, double offset, double amp_) {
  const double amp = 8;

  double deltaL = ( LF.get_position() + LB.get_position() ) / 2;
  double deltaR = ( RF.get_position() + RB.get_position() ) / 2;

  switch(mode) {
    case 1:
      return (deltaL - deltaR + offset) / ( amp + amp_ );
      break;

    case 2:
      deltaL = ( LF.get_position() - LB.get_position() ) / 2;
      deltaR = ( RF.get_position() - RB.get_position() ) / 2;
      return ( deltaL - deltaR ) / ( amp + amp_ ) + offset;
      break;

    default:
      return ( deltaL - deltaR ) / amp;
      break;
  }
}



void wait(int ms) {
  pros::delay(ms);
}

void print(const char * text) {
  std::cout << text << std::endl;
}
