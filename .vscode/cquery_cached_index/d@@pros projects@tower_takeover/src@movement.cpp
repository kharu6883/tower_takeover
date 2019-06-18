#include "main.h"
#include "config.h"

void drive(double target, int speed) {
  int dir; // 1 - forward, 0 - reverse

  if(target > 0) dir = 1;
  if(target < 0) dir = 0;

  switch(dir) {
    case 1:

      break;

    case 0:
      
      break;
  }
}

void moveClaw(double rotation, int speed) {
  ClawL.move_absolute(rotation, speed);
  ClawR.move_absolute(rotation, -speed);
}
