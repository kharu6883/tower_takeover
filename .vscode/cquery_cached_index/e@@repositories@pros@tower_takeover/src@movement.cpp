#include "main.h"
#include "config.h"

void moveClaw(double rotation, int speed) {
  ClawL.move_absolute(rotation, speed);
  ClawR.move_absolute(rotation, -speed);
}
