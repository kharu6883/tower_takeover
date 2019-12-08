#include "lib_7k/util/odometry.h"

void Odometry::start(void *ignore) {
  if(!isRunning) {
    pros::delay(500);
    Odometry *that = static_cast<Odometry*>(ignore);
    that -> run();
  }
}

void Odometry::run() {
  isRunning = true;

  while(isRunning) {
    
    pros::delay(20);
  }
}

void Odometry::stop() {
  isRunning = false;
}
