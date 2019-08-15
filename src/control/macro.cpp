#include "main.h"

#include "config/motor.h"
#include "config/io.h"

#include "control/macro.h"
#include "control/drive.h"

bool isStacking = false;

const double kP = 60;



void rack(int speed) {
  Rack.move_velocity(speed);
}

void arm(int speed) {
  Arm.move_velocity(speed);
}

void roller(int speed) {
  RollerL.move_velocity(speed);
  RollerR.move_velocity(speed);
}

void roller(double rot, int speed) {
  RollerL.move_relative(rot, speed);
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
  const double kP = 100;

  double tolerance = 3;

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

  while(target < Rack.get_position()) { // Goin' down
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

void tower() {
  const double kP = 200;
  double rackTarget, armTarget, tolerance = 2.5;

  //roller(200);
  //wait(1000);
  roller(-60);
  wait(600);
  roller(0);

  drive(-200, 50, 9);

  while(true) {
    armTarget = pTerm(0, abs(Arm.get_position()), kP);
    arm(armTarget);

    if(isSettled(armTarget, tolerance)) { arm(0); break; }
    wait(20);
  }

  roller(-1.5, 200);
  while(true) {
    armTarget = pTerm(2.509, abs(Arm.get_position()), kP);
    arm(armTarget);

    if(isSettled(armTarget, tolerance)) { arm(0); break; }
  }

  roller(-2, 200);
  wait(1000);
  while(true) {
    armTarget = pTerm(0, abs(Arm.get_position()), kP);
    arm(armTarget);

    if(isSettled(armTarget, tolerance)) { arm(0); break; }
    wait(20);
  }
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

double slop() {
  const double amp = 8;

  double deltaL = ( LF.get_position() + LB.get_position() ) / 2;
  double deltaR = ( RF.get_position() + RB.get_position() ) / 2;

  return ( deltaL - deltaR ) / amp;
}

double slop(int mode, double offset) {
  const double amp = 8;

  double deltaL = ( LF.get_position() + LB.get_position() ) / 2;
  double deltaR = ( RF.get_position() + RB.get_position() ) / 2;

  switch(mode) {
    case 1:
      return (deltaL + deltaR) * amp;
      break;

    case 2:
      deltaL = ( LF.get_position() - LB.get_position() ) / 2;
      deltaR = ( RB.get_position() - RF.get_position() ) / 2;
      return ( deltaL - deltaR ) / amp + offset;
      break;

    default:
      return ( deltaL - deltaR ) / amp;
  }
}



void wait(int ms) {
  pros::delay(ms);
}

void print(const char * text) {
  std::cout << text << std::endl;
}
