#include "main.h"

void reset() {
  LF.tare_position();
  LB.tare_position();
  RF.tare_position();
  RB.tare_position();
  left(0);
  right(0);
}

void drive(double target, int speed) {
  const double kP = 70;
  const double kD = 2;

  double current, error, last, dTerm, output;

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
    delay(60);
  }

  while(target < 0) {
    current = LF.get_position();
    error = target - current;
    dTerm = error - last;
    last = error;

    output = ( error * kP ) + ( dTerm * kD );

    if(output > speed) output = speed;

    slew(output);
  }
}

void slew(int target) {
  const double accel = .1;

  int output = 0;

  if(output < target) {
    output += accel;
  }

  if(output > target) {
    output = target;
  }

  left(output);
  right(output);
}

void left(int speed) {
  LF.move(speed);
  LB.move(speed);
}

void right(int speed) {
  RF.move(speed);
  RB.move(speed);
}

void moveClaw(double rotation, int speed) {
  ClawL.move_absolute(rotation, speed);
  ClawR.move_absolute(rotation, -speed);
}
