#include "main.h"
#include "config.h"

double current, error, last, derivative, output;

void reset() {
  current = 0; error = 0; last = 0; derivative = 0; output = 0;

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

  double lastSlew = 0, nowSlew = 0;

  reset();

  while(target > 0 && isMoving()) {
    wait(20);
    lastSlew = nowSlew;
    current = LF.get_position();
    error = target - current;

    output = pTerm(target, LF.get_position(), kP) + dTerm(error, last);

    last = error;

    if(output > speed) output = speed;

    nowSlew = slew(output, lastSlew, 9);

    left(nowSlew);
    right(nowSlew);
  }
}

void slop() {
  int error = 0;
}

void left(int speed) {
  LF.move_velocity(speed);
  LB.move_velocity(speed);
}

void right(int speed) {
  RF.move_velocity(speed);
  RB.move_velocity(speed);
}
