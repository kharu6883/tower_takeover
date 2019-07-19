#include "main.h"
#include "config.h"

bool isStacking = false;

const double kP = 40;

// Basic Movement
void roller(int speed) {
  flap(speed);
}

void roller(int speed, bool suck) {
  while(suck) {
    flap(speed);
  }

  while(!suck) {
    flap(-speed);
  }
}

// Complex Movement
void lift(double target, int speed) {
  while(target > 0) {
    double output = pTerm(target, LiftL.get_position(), 3);

    if(output > speed) output = speed;
    lift(output);

    if(output < 1) break;
  }

  while(target < 0) {
    double output = pTerm(target, LiftL.get_position(), 3);

    if(abs(output) > speed) output = speed;
    lift(-output);

    if(abs(output) < 1) break;
  }
}

// PID Calculation
double pTerm(double target, double sensor, double kP) {
  return(target - sensor) * kP;
}

double dTerm(double now, double last) {
  return now - last;
}

// Etc
void wait(int ms) { // Just does pros::delay, so nothing to worry about
  pros::delay(ms);
}

void print(const char * text) {
  std::cout << text << std::endl;
}
