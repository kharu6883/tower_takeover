#include "main.h"

#include "config/io.h"
#include "control/displayController.h"
using namespace Display;

static int count = 0;

RemoteDisplay::RemoteDisplay() {

}

void RemoteDisplay::update() {
  while(true) {
    master.print(0, 0, "Yee haw");
    pros::delay(50);
  }
}

void RemoteDisplay::run(void *ignore) {
  pros::delay(500);
  RemoteDisplay* that = static_cast<RemoteDisplay*>(ignore);
  that -> update();
}
