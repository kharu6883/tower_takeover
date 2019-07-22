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
  while(target > 0) {
    double output = pTerm(target, Rack.get_position(), 3);

    if(output > speed) output = speed;
    rack(output);

    if(output < 1) break;
  }

  while(target < 0) {
    double output = pTerm(target, Rack.get_position(), 3);

    if(abs(output) > speed) output = speed;
    rack(-output);

    if(abs(output) < 1) break;
  }
}

void armAsync(double target, int speed, double rate) {

}



double pTerm(double target, double sensor, double kP) {
  return(target - sensor) * kP;
}

double dTerm(double now, double last) {
  return now - last;
}

double slew(double target, double actual, double rate) {
  double output;

  if(target > actual + rate) {
    output = actual;
    output += rate;
  } else {
    output = target;
  }

  return output;
}

void wait(int ms) {
  pros::delay(ms);
}

void print(const char * text) {
  std::cout << text << std::endl;
}
