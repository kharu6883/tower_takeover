#include "main.h"
using namespace okapi;
using namespace path;
using namespace std;

int autonSlot = 0; // 0 = first slot;

map<int, void(*)(void)> Autonomous;
map<int, const char *> SlotName;
map<int, const char *>::iterator SlotIter;

void autonomous() {
  Autonomous[autonSlot]();
}

void initAuton() { // The autons will be stored in this order, starting from 0.
  addAuton("Test Auton", autonboi);
  addAuton("Motion Test", motionTest);

  print("Auton initialized!");
}

void addAuton(const char * autonName, void(*function)()) {
  Autonomous.insert(make_pair(Autonomous.size(), function));
  SlotName.insert(make_pair(SlotName.size(), autonName));
}

void setAuton(int slot) {
  autonSlot = slot;
}

const char * getName(int slot) {
  return SlotName[slot];
}
