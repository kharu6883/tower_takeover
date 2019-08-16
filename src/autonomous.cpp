#include "main.h"

#include "control/asyncController.h"
#include "control/autonController.h"
#include "control/path.h"
using namespace okapi;
using namespace path;
using namespace std;

int autonSlot = 1; // 0 = first slot;

map<int, void(*)(void)> Autonomous;
map<int, const char *> SlotName;
map<int, const char *>::iterator SlotIter;

ControlAsync Start;

void autonomous() {
  pros::Task asyncDrive(Start.run);
  Autonomous[autonSlot]();
}

void initAuton() { // The autons will be stored in this order, starting from 0.
  addAuton("Test Auton", tester);


  addAuton("Red Small zone 8 cubes", r_s_8); // Slot 1
  addAuton("Skills 1", skills1);


  // Running some tests below
  addAuton("Motion Test", motionTest);
}

void addAuton(const char * autonName, void(*function)()) {
  Autonomous.insert(make_pair(Autonomous.size(), function));
  SlotName.insert(make_pair(SlotName.size(), autonName));
}

void setAuton(int slot) {
  autonSlot = slot;
}

int getSlot() {
  return autonSlot;
}

const char * getName(int slot) {
  return SlotName[slot];
}
