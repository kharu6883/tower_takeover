#include "main.h"

#include "config/io.h"
#include "control/autonController.h"
#include "control/displayController.h"
using namespace Display;

static Autonomous Auton;
static int count = 0;

bool RemoteDisplay::isSetting = false;
const char *RemoteDisplay::text = "";

okapi::Controller(controller);

RemoteDisplay::RemoteDisplay() {

}

void RemoteDisplay::update() {
  int type = 1, slot = 0, limit = Auton.getSize(type);
  int lastType, lastSlot;

  while(true) {
    if(master.get_digital_new_press(DIGITAL_B)) type++;
    if(type > 3) type = 0;

    limit = Auton.getSize(type);

    if(master.get_digital_new_press(DIGITAL_Y)) slot++;
    if(slot > limit) slot = 0;

    if(type != lastType || slot != lastSlot) {
      setText(Auton.getAbbv(type, slot).c_str());
    }

    if(master.get_digital_new_press(DIGITAL_A)) {
      Auton.setType(type);
      Auton.setSlot(slot);
    }

    lastType = type;
    lastSlot = slot;

    if(isSetting) {
      master.set_text(0, 0, text);
      isSetting = false;
    }

    pros::delay(1000);
  }
}

void RemoteDisplay::setText(const char *set) {
  // std::string applier;
  // applier.append(set);
  // applier.append("            ");
  text = set;
  isSetting = true;
}

void RemoteDisplay::run(void *ignore) {
  pros::delay(500);
  RemoteDisplay* that = static_cast<RemoteDisplay*>(ignore);
  that -> update();
}
