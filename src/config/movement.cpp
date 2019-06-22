#include "main.h"
#include "config.h"

double current, error, last, dTerm, output;

void reset() {
  current = 0; error = 0; last = 0; dTerm = 0; output = 0;

  LF.tare_position();
  LB.tare_position();
  RF.tare_position();
  RB.tare_position();
  left(0);
  right(0);
}

void drive(double target, int speed, double rate) {
  const double kP = 70;
  const double kD = 2;

  reset();

  while(target > 0) {
    current = LF.get_position();
    error = target - current;
    dTerm = error - last;
    last = error;

    output = ( error * kP ) + ( dTerm * kD );

    if(output > speed) output = speed;

    left(output);
    right(output);
    wait(1);
  }

  while(target < 0) {
    current = LF.get_position();
    error = target - current;
    dTerm = error - last;
    last = error;

    output = ( error * kP ) + ( dTerm * kD );

    if(output > speed) output = speed;

    slew(output, rate);
  }
}

void slew(int target, double rate) {
  output = 0;

  if(output < target) {
    output += rate;
    wait(1);
  }

  if(output > target) output = target;

  left(output);
  right(output);
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
