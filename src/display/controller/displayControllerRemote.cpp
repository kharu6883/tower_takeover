#include "main.h"

#include "config/io.h"
#include "control/displayController.h"
using namespace Display;

static int count = 0;

RemoteDisplay::RemoteDisplay() {

}

void RemoteDisplay::update() {
  while(true) {
    

    pros::delay(50);
  }
}

void RemoteDisplay::run(void *ignore) {
  pros::delay(500);
  BrainDisplay* that = static_cast<BrainDisplay*>(ignore);
  that -> update();
}
