#include "main.h"

#include "config/motor.h"
#include "config/io.h"

#include "control/drive.h"
#include "control/macro.h"

void reset() {
  LF.tare_position();
  LB.tare_position();
  RF.tare_position();
  RB.tare_position();
  left(0);
  right(0);
}

void drive(double target, int speed, double rate) {
  double current, error, last, derivative, output;

  const double kP = 0.6;
  const double kD = 0.6;

  double deltaL, deltaR;
  double slewOutput = 0;

  reset();

  while(target > 0) {
    deltaL = (LF.get_position() + LB.get_position()) / 2;
    deltaR = (RF.get_position() + RB.get_position()) / 2;
    current = ( deltaL + deltaR ) / 2;
    error = target - current;

    output = pTerm(target, current, kP) + dTerm(error, last) * kD;

    last = error;

    if(output > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = output;
    }

    if(slewOutput > speed) slewOutput = speed;

    if(isSettled(error, 9)) break;

    left(slewOutput - slop(0));
    right(slewOutput + slop(0));

    std::cout << LF.get_position() << ", " << RF.get_position() << std::endl;
    wait(20);
  }

  while(target < 0) {
    deltaL = (LF.get_position() + LB.get_position()) / 2;
    deltaR = (RF.get_position() + RB.get_position()) / 2;
    current = ( deltaL + deltaR ) / 2;
    error = target - current;

    output = pTerm(target, current, kP) + dTerm(error, last) * kD;

    last = error;

    if(abs(output) > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = abs(output);
    }

    if(slewOutput > speed) slewOutput = speed;

    if(isSettled(abs(error), 9)) break;

    left(-slewOutput - slop(0));
    right(-slewOutput + slop(0));

    std::cout << LF.get_position() << ", " << RF.get_position() << std::endl;
    wait(20);
  }

  reset();
}

void drive(double target, int speed, double rate, double amp, double offset) {
  double current, error, last, derivative, output;

  const double kP = 0.6;
  const double kD = 0.6;

  double deltaL, deltaR;
  double slewOutput = 0;

  reset();

  while(target > 0) {
    deltaL = (LF.get_position() + LB.get_position()) / 2;
    deltaR = (RF.get_position() + RB.get_position()) / 2;
    current = ( deltaL + deltaR ) / 2;
    error = target - current;

    output = pTerm(target, current, kP) + dTerm(error, last) * kD;

    last = error;

    if(output > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = output;
    }

    if(slewOutput > speed) slewOutput = speed;

    if(isSettled(error, 9)) break;

    left(slewOutput - slop(1, offset, amp));
    right(slewOutput + slop(1, offset, amp));

    std::cout << LF.get_position() << ", " << RF.get_position() << std::endl;
    wait(20);
  }

  while(target < 0) {
    deltaL = (LF.get_position() + LB.get_position()) / 2;
    deltaR = (RF.get_position() + RB.get_position()) / 2;
    current = ( deltaL + deltaR ) / 2;
    error = target - current;

    output = pTerm(target, current, kP) + dTerm(error, last) * kD;

    last = error;

    if(abs(output) > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = abs(output);
    }

    if(slewOutput > speed) slewOutput = speed;

    if(isSettled(abs(error), 9)) break;

    left(-slewOutput - slop(1, offset, amp));
    right(-slewOutput + slop(1, offset, amp));

    std::cout << LF.get_position() << ", " << RF.get_position() << std::endl;
    wait(20);
  }

  reset();
}

void turn(double target, int speed, double rate) {
  double current, error, last, derivative, output;

  const double kP = 0.6;
  const double kD = 0.6;

  double deltaL, deltaR;
  double lastSlew = 0, nowSlew = 0, slewOutput = 0;

  Slew slew(rate);

  reset();

  while(target > 0) { // Turn Right
    deltaL = (LF.get_position() + LB.get_position()) / 2;
    deltaR = (RF.get_position() + RB.get_position()) / 2;
    current = ( deltaL + abs(deltaR) ) / 2;
    error = target - current;

    output = pTerm(target, current, kP) + dTerm(error, last) * kD;

    last = error;

    slew.calculate(output);

    if(slew.getOutput() > speed) slew.setOutput(speed);

    if(isSettled(error, 6)) break;

    left(slew.getOutput());
    right(-slew.getOutput());

    std::cout << LF.get_position() << ", " << RF.get_position() << std::endl;
    wait(20);
  }

  while(target < 0) { // Turn Right
    deltaL = (LF.get_position() + LB.get_position()) / 2;
    deltaR = (RF.get_position() + RB.get_position()) / 2;
    current = ( abs(deltaL) + deltaR ) / 2;
    error = target + current;

    output = pTerm(target, -current, kP) + dTerm(error, last) * kD;

    last = error;

    slew.calculate(abs(output));

    if(slew.getOutput() > speed) slew.setOutput(speed);

    if(isSettled(error, 6)) break;

    left(-slew.getOutput());
    right(slew.getOutput());

    std::cout << LF.get_position() << ", " << RF.get_position() << std::endl;
    wait(20);
  }

  reset();
}

void strafe(double target, int speed, double rate) {
  double current, error, last, derivative, output;

  const double kP = 0.5;
  const double kD = 0.6;

  double deltaL, deltaR;
  double lastSlew = 0, nowSlew = 0, slewOutput = 0;

  reset();

  while(target > 0) {
    deltaL = (LF.get_position() - LB.get_position()) / 2;
    deltaR = (RF.get_position() - RB.get_position()) / 2;
    current = (deltaL + deltaR) / 2;
    error = target - current;

    output = pTerm(target, current, kP) + dTerm(error, last) * kD;

    last = error;

    if(output > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = output;
    }

    if(slewOutput > speed) slewOutput = speed;

    LF.move_velocity(slewOutput - slop(2, 0, 0));
    LB.move_velocity(-slewOutput + slop(2, 0, 0));
    RF.move_velocity(slewOutput - slop(2, 0, 0));
    RB.move_velocity(-slewOutput + slop(2, 0, 0));

    if(isSettled(error, 6)) break;
    wait(20);
  }

  while(target < 0) {
    deltaL = (LF.get_position() - LB.get_position()) / 2;
    deltaR = (RF.get_position() - RB.get_position()) / 2;
    current = (deltaL + deltaR) / 2;
    error = target - current;

    output = pTerm(target, current, kP) + dTerm(error, last) * kD;

    last = error;

    if(abs(output) > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = abs(output);
    }

    if(slewOutput > speed) slewOutput = speed;

    LF.move_velocity(-slewOutput - slop(2, 0, 0));
    LB.move_velocity(slewOutput + slop(2, 0, 0));
    RF.move_velocity(-slewOutput - slop(2, 0, 0));
    RB.move_velocity(slewOutput + slop(2, 0, 0));

    if(isSettled(abs(error), 6)) break;
    wait(20);
  }

  reset();
}

void strafe(double target, int speed, double rate, double sturn) {
  double current, error, last, derivative, output;

  const double kP = 0.6;
  const double kD = 0.6;

  double deltaL, deltaR;
  double lastSlew = 0, nowSlew = 0, slewOutput = 0;

  reset();

  while(target > 0) {
    deltaL = (LF.get_position() - LB.get_position()) / 2;
    deltaR = (RF.get_position() - RB.get_position()) / 2;
    current = (deltaL + deltaR) / 2;
    error = target - current;

    output = pTerm(target, current, kP) + dTerm(error, last) * kD;

    last = error;

    if(output > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = output;
    }

    if(slewOutput > speed) slewOutput = speed;

    LF.move_velocity(slewOutput - slop(2, sturn, 0));
    LB.move_velocity(-slewOutput + slop(2, sturn, 0));
    RF.move_velocity(slewOutput - slop(2, sturn, 0));
    RB.move_velocity(-slewOutput + slop(2, sturn, 0));

    if(isSettled(error, 6)) break;
    wait(20);
  }

  while(target < 0) {
    deltaL = (LF.get_position() - LB.get_position()) / 2;
    deltaR = (RF.get_position() - RB.get_position()) / 2;
    current = (deltaL + deltaR) / 2;
    error = target - current;

    output = pTerm(target, current, kP) + dTerm(error, last) * kD;

    last = error;

    if(abs(output) > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = abs(output);
    }

    if(slewOutput > speed) slewOutput = speed;

    LF.move_velocity(-slewOutput - slop(2, -sturn, 0));
    LB.move_velocity(slewOutput + slop(2, -sturn, 0));
    RF.move_velocity(-slewOutput - slop(2, -sturn, 0));
    RB.move_velocity(slewOutput + slop(2, -sturn, 0));

    if(isSettled(abs(error), 6)) break;
    wait(20);
  }

  reset();
}


void align(double target, double tolerance) {

  double errorL, errorR, lastL, lastR, outputL, outputR;
  double now, last, elapsed;

  double kP = 0.7, kI = 1, kD = 8;

  reset();

  while(true) {
    elapsed = now - last;
    now = clock();

    errorL = target - ultraL.get_value();
    errorR = target - ultraR.get_value();

    outputL = pTerm(target, ultraL.get_value(), kP) + kD * dTerm(errorL, lastL) * elapsed;
    outputR = pTerm(target, ultraR.get_value(), kP) + kD * dTerm(errorR, lastR) * elapsed;

    left(outputL);
    right(outputR);

    lastL = errorL;
    lastR = errorR;

    last = clock();

    if(isSettled(outputL, tolerance) && isSettled(outputR, tolerance)) break;
  }

  reset();
}

void lockChassis() {
  LF.set_brake_mode(MOTOR_BRAKE_HOLD);
  LB.set_brake_mode(MOTOR_BRAKE_HOLD);
  RF.set_brake_mode(MOTOR_BRAKE_HOLD);
  RB.set_brake_mode(MOTOR_BRAKE_HOLD);
}

void unlockChassis() {
  LF.set_brake_mode(MOTOR_BRAKE_COAST);
  LB.set_brake_mode(MOTOR_BRAKE_COAST);
  RF.set_brake_mode(MOTOR_BRAKE_COAST);
  RB.set_brake_mode(MOTOR_BRAKE_COAST);
}

void left(int speed) {
  LF.move_velocity(speed);
  LB.move_velocity(speed);
}

void right(int speed) {
  RF.move_velocity(speed);
  RB.move_velocity(speed);
}
