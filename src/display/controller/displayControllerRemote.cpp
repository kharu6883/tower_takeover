#include "main.h"

#include "config/io.h"
#include "control/autonController.h"
#include "control/displayController.h"
using namespace Display;

static Autonomous Auton;

const char *RemoteDisplay::text = "";

RemoteDisplay::RemoteDisplay() {}

void RemoteDisplay::update() {
  int lastType, lastSlot;

  while(true) {
    if(Auton.getType() != lastType || Auton.getSlot() != lastSlot) {
      std::string setter = Auton.getAbbv(Auton.getType(), Auton.getSlot()) + "       ";
      master.set_text(0, 0, setter.c_str());
      lastType = Auton.getType();
      lastSlot = Auton.getSlot();
    }

    pros::delay(20);
  }
}

void RemoteDisplay::setText(std::string text_) {
  text_.append("");
  text = text_.c_str();
}

void RemoteDisplay::run(void *ignore) {
  pros::delay(500);
  RemoteDisplay* that = static_cast<RemoteDisplay*>(ignore);
  that -> update();
}
