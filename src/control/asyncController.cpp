#include "main.h"

ControlAsync::ControlAsync() {

}

void ControlAsync::run(void* args) {

  reset();

  while(true) {
    if(isDrive) {
      drive(target.length, target.speed, target.rate);
    }

    if(isTurn) {
      turn(target.length, target.speed, target.rate);
    }

    wait(20);
  }
}

void ControlAsync::reset() {
  stop();
  rack(0);
  arm(0);
  target = {0, 0, 0};
}

void ControlAsync::stop() {
  isDrive = false;
  isTurn = false;
}

bool ControlAsync::isDisabled() {
  if(isDrive || isTurn) {
    return false;
  } else {
    return true;
  }
}

void ControlAsync::driveAsync(double length, int speed, int rate) {
  this -> target.length = length;
  this -> target.speed = speed;
  this -> target.rate = rate;
  isDrive = true;
}

void ControlAsync::turnAsync(double length, int speed, int rate) {
  this -> target.length = length;
  this -> target.speed = speed;
  this -> target.rate = rate;
  isTurn = true;
}
