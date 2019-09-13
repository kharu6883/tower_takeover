#include "main.h"

#include "config/io.h"
#include "control/displayController.h"
using namespace Display;

static int count = 0;

bool RemoteDisplay::isSetting = false;
const char *RemoteDisplay::text = "";

RemoteDisplay::RemoteDisplay() {

}

void RemoteDisplay::update() {
  while(true) {
    if(isSetting) {
      master.print(0, 0, text);
      isSetting = false;
    }

    pros::delay(50);
  }
}

void RemoteDisplay::setText(const char *set) {
  std::string applier;
  applier.append(set);
  applier.append("            ");
  text = applier.c_str();
  isSetting = true;
}

void RemoteDisplay::run(void *ignore) {
  pros::delay(500);
  RemoteDisplay* that = static_cast<RemoteDisplay*>(ignore);
  that -> update();
}
