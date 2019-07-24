#include "main.h"
#include "config.h"

bool isStacking = false;

const double kP = 60;



void rack(int speed) {
  Rack.move_velocity(speed);
}

void arm(int speed) {
  Arm.move_velocity(speed);
}

void roller(int speed) {
  FlapL.move_velocity(speed);
  FlapR.move_velocity(-speed);
}



void rackAsync(double target, int speed, double rate) {
  const double kP = 100;

  double tolerance = 3;

  double slewOutput;

  while(target > Rack.get_position()) { // Goin' up
    double desired = pTerm(target, Rack.get_position(), kP);

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

  while(target < Rack.get_position()) { // Goin' down
    double desired = pTerm(target, Rack.get_position(), kP);

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

void armAsync(double target, int speed, double rate) {
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

void tower(int id) {
  const double kP = 200;
  double rackTarget, armTarget, tolerance = 1;

  //roller(200);
  //wait(1000);
  roller(-60);
  wait(600);
  roller(0);

  drive(-200, 50, 9);

  while(id == 1) { // Low Tower
    rackTarget = pTerm(1.212, abs(Rack.get_position()), kP);
    rack(rackTarget);

    armTarget = pTerm(3.1, abs(Arm.get_position()), kP);
    arm(armTarget);

    if(isSettled(rackTarget, tolerance) && isSettled(armTarget, tolerance)) break;
  }

  while(id == 2) { // Middle tower
    rackTarget = pTerm(2.2, abs(Rack.get_position()), kP);
    rack(rackTarget);

    armTarget = pTerm(4, abs(Arm.get_position()), kP);
    arm(armTarget);

    if(isSettled(rackTarget, tolerance) && isSettled(armTarget, tolerance)) break;
  }

  drive(150, 30, 9);
  wait(200);
  roller(-100);
  wait(1000);
  roller(0);
  drive(-100, 30, 9);

  while(true) { // Middle tower
    rackTarget = pTerm(0, abs(Rack.get_position()), kP);
    rack(rackTarget);

    armTarget = pTerm(0, abs(Arm.get_position()), kP);
    arm(armTarget);

    if(isSettled(rackTarget, tolerance) && isSettled(armTarget, tolerance)) break;
  }
}



double pTerm(double target, double sensor, double kP) {
  return(target - sensor) * kP;
}

double dTerm(double now, double last) {
  return now - last;
}

bool isSettled(double error, double tolerance) {
  bool settled = false;

  if(error > -tolerance && error < tolerance) settled = true;

  return settled;
}



void wait(int ms) {
  pros::delay(ms);
}

void print(const char * text) {
  std::cout << text << std::endl;
}
