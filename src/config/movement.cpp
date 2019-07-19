#include "main.h"
#include "config.h"

double current, error, last, derivative, output, slewOutput;

void reset() {
  current = 0; error = 0; last = 0; derivative = 0; output = 0; slewOutput = 0;

  LF.tare_position();
  LB.tare_position();
  RF.tare_position();
  RB.tare_position();
  left(0);
  right(0);
}

bool isMoving() {
  static int now, last = 0;

  now = LF.get_position();

  return true;
}

void drive(double target, int speed, double rate) {
  const double kP = 70;
  const double kD = 2;

  reset();

  while(target > 0 && isMoving()) {
    wait(20);
    current = LF.get_position();
    error = target - current;

    output = pTerm(target, LF.get_position(), kP) + dTerm(error, last);

    last = error;

    if(output > speed) output = speed;

    slew(output, rate);
  }
}

void slew(int target, double rate) {
  if(target > slewOutput + rate) {
    slewOutput += rate;
  } else {
    slewOutput = target;
  }

  left(abs(slewOutput));
  right(abs(slewOutput));
}

void slop() {
  int error = 0;
}

void left(int speed) {
  LF.move(speed);
  LB.move(speed);
}

void right(int speed) {
  RF.move(speed);
  RB.move(speed);
}

void lift(int speed) {
  LiftL.move(speed);
  LiftR.move(speed);
}

void flap(int speed) {
  FlapL.move(speed);
  FlapR.move(-speed);
}
