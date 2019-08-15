#include "main.h"

#include "config/motor.h"
#include "control/asyncController.h"
#include "control/drive.h"
#include "control/macro.h"

bool ControlAsync::isDrive = false;
bool ControlAsync::isTurn = false;
bool ControlAsync::isStrafe = false;

double ControlAsync::sturn = 0;

double ControlAsync::current = 0,
ControlAsync::error = 0,
ControlAsync::last = 0,
ControlAsync::derivative = 0,
ControlAsync::output = 0,
ControlAsync::slewOutput = 0;

Vector2 ControlAsync::target = {0, 0, 0};

ControlAsync::ControlAsync() {}

void ControlAsync::run(void* args) {

  reset();

  const double kP = 0.6;
  const double kD = 0.6;

  double deltaL, deltaR;

  while(true) {

    /*===========================================
      DRIVING
    ===========================================*/

    if(isDrive) {
      if(target.length > 0) {
        deltaL = (LF.get_position() + LB.get_position()) / 2;
        deltaR = (RF.get_position() + RB.get_position()) / 2;
        current = ( deltaL + deltaR ) / 2;
        error = target.length - current;

        output = pTerm(target.length, current, kP) + dTerm(error, last) * kD;

        last = error;

        if(output > slewOutput + target.rate) {
          slewOutput += target.rate;
        } else {
          slewOutput = output;
        }

        if(slewOutput > target.speed) slewOutput = target.speed;

        left(slewOutput - slop());
        right(slewOutput + slop());

        if(isSettled(error, 9)) { reset(); isDrive = false; }
      }

      if(target.length < 0) {
        deltaL = (LF.get_position() + LB.get_position()) / 2;
        deltaR = (RF.get_position() + RB.get_position()) / 2;
        current = ( deltaL + deltaR ) / 2;
        error = target.length - current;

        output = pTerm(target.length, current, kP) + dTerm(error, last) * kD;

        last = error;

        if(abs(output) > slewOutput + target.rate) {
          slewOutput += target.rate;
        } else {
          slewOutput = abs(output);
        }

        if(slewOutput > target.speed) slewOutput = target.speed;

        left(-slewOutput - slop());
        right(-slewOutput + slop());

        if(isSettled(abs(error), 9)) { reset(); isDrive = false; }
      }
    }

    /*===========================================
      TURNING
    ===========================================*/

    if(isTurn) {
      if(target.length > 0) { // Turn Right
        deltaL = (LF.get_position() + LB.get_position()) / 2;
        deltaR = (RF.get_position() + RB.get_position()) / 2;
        current = ( deltaL + abs(deltaR) ) / 2;
        error = target.length - current;

        output = pTerm(target.length, current, kP) + dTerm(error, last) * kD;

        last = error;

        if(output > slewOutput + target.rate) {
          slewOutput += target.rate;
        } else {
          slewOutput = output;
        }

        if(slewOutput > target.speed) slewOutput = target.speed;

        left(slewOutput);
        right(-slewOutput);

        if(isSettled(error, 6)) { reset(); isTurn = false; }
      }

      if(target.length < 0) { // Turn Left
        deltaL = (LF.get_position() + LB.get_position()) / 2;
        deltaR = (RF.get_position() + RB.get_position()) / 2;
        current = ( abs(deltaL) + deltaR ) / 2;
        error = target.length + current;

        output = pTerm(target.length, -current, kP) + dTerm(error, last) * kD;

        last = error;

        if(abs(output) > slewOutput + target.rate) {
          slewOutput += target.rate;
        } else {
          slewOutput = abs(output);
        }

        if(slewOutput > target.speed) slewOutput = target.speed;

        left(-slewOutput);
        right(slewOutput);

        if(isSettled(error, 6)) { reset(); isTurn = false; }
      }
    }

    /*===========================================
      STRAFING
    ===========================================*/

    if(isStrafe) {
      if(target.length > 0) {
        deltaL = (LF.get_position() - LB.get_position()) / 2;
        deltaR = (RB.get_position() - RF.get_position()) / 2;
        current = (deltaL + deltaR) / 2;
        error = target.length - current;

        output = pTerm(target.length, current, kP) + dTerm(error, last) * kD;

        last = error;

        if(output > slewOutput + target.rate) {
          slewOutput += target.rate;
        } else {
          slewOutput = output;
        }

        if(slewOutput > target.speed) slewOutput = target.speed;

        LF.move_velocity(slewOutput + slop(2, sturn));
        LB.move_velocity(-slewOutput + slop(2, sturn));
        RF.move_velocity(-slewOutput - slop(2, sturn));
        RB.move_velocity(slewOutput - slop(2, sturn));

        if(isSettled(error, 6)) { reset(); isStrafe = false; }
      }

      if(target.length < 0) {
        deltaL = (LF.get_position() - LB.get_position()) / 2;
        deltaR = (RB.get_position() - RF.get_position()) / 2;
        current = (deltaL + deltaR) / 2;
        error = target.length - current;

        output = pTerm(target.length, current, kP) + dTerm(error, last) * kD;

        last = error;

        if(abs(output) > slewOutput + target.rate) {
          slewOutput += target.rate;
        } else {
          slewOutput = abs(output);
        }

        if(slewOutput > target.speed) slewOutput = target.speed;

        LF.move_velocity(-slewOutput + slop(2, -sturn));
        LB.move_velocity(slewOutput + slop(2, -sturn));
        RF.move_velocity(slewOutput - slop(2, -sturn));
        RB.move_velocity(-slewOutput - slop(2, -sturn));

        if(isSettled(abs(error), 6)) { reset(); isStrafe = false; }
      }
    }

    wait(20);
  }
}

void ControlAsync::reset() {
  current = 0; error = 0; last = 0; derivative = 0; output = 0; slewOutput = 0;

  LF.tare_position();
  LB.tare_position();
  RF.tare_position();
  RB.tare_position();

  left(0);
  right(0);

  ControlAsync::target = {0, 0, 0};
  sturn = 0;
}

void ControlAsync::stop() {
  ControlAsync::isDrive = false;
  ControlAsync::isTurn = false;
  ControlAsync::isStrafe = false;
}

bool ControlAsync::isDisabled() {
  if(isDrive || isTurn || isStrafe) {
    return false;
  } else {
    return true;
  }
}

void ControlAsync::drive(double length, int speed, int rate) {
  reset();
  this -> target.length = length;
  this -> target.speed = speed;
  this -> target.rate = rate;
  isDrive = true;
}

void ControlAsync::turn(double length, int speed, int rate) {
  reset();
  this -> target.length = length;
  this -> target.speed = speed;
  this -> target.rate = rate;
  isTurn = true;
}

void ControlAsync::strafe(double length, int speed, int rate) {
  reset();
  this -> target.length = length;
  this -> target.speed = speed;
  this -> target.rate = rate;
  isStrafe = true;
}

void ControlAsync::strafe(double length, int speed, int rate, double sturn) {
  reset();
  this -> target.length = length;
  this -> target.speed = speed;
  this -> target.rate = rate;
  this -> sturn = sturn;
  isStrafe = true;
}
