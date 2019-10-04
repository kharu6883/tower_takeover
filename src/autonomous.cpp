#include "main.h"

#include "control/asyncController.h"
#include "control/autonController.h"
#include "control/path.h"
using namespace okapi;
using namespace path;
using namespace std;

static ControlAsync Control;

bool Autonomous::isInitialized = false;
int Autonomous::autonSlot = 1;

std::map<int, void(*)(void)> Autonomous::AutonArray;
std::map<int, const char *> Autonomous::SlotName;

void autonomous() {
  Control.resume();
  Autonomous Auton;
  Auton.runAuton();
}

Autonomous::Autonomous() { // The autons will be stored in this order, starting from 0.
  if(!isInitialized) {
    addAuton("Test Auton", tester); // Slot 0

    // Slot 1
    addAuton("Red Small zone 8 cubes", r_s_8);
    addAuton("Red Big zone 8 cubes", r_b_8);
    addAuton("Blue Small zone 8 cubes", b_s_8);
    addAuton("Blue Big zone 8 cubes", b_b_8);
    addAuton("Skills 1", skills1);

    // Slot 5

    isInitialized = true;
  }
}

void Autonomous::runAuton() {
  Autonomous::AutonArray[autonSlot]();
}

void Autonomous::addAuton(const char * autonName, void(*function)()) {
  AutonArray.insert(make_pair(AutonArray.size(), function));
  SlotName.insert(make_pair(SlotName.size(), autonName));
}

int Autonomous::getSlot() {
  return autonSlot;
}

void Autonomous::setSlot(int slot) {
  autonSlot = slot;
}

int Autonomous::getSize() {
  return SlotName.size();
}

const char * Autonomous::getName(int slot) {
  return SlotName[slot];
}
