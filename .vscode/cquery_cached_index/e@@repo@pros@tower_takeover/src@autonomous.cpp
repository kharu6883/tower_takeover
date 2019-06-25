#include "main.h"
#include <vector>
using namespace okapi;
using namespace path;
using namespace std;

int autonSlot = 1; // 0 = first slot;

map<int, void(*)(void)> Autonomous;
map<int, const char *> SlotName;

void autonomous() {
  Autonomous[autonSlot]();
}

void initAuton() {
  addAuton(0, "Test Auton", autonboi);
  addAuton(1, "Motion Test", motionTest);
}

void addAuton(int slot, const char * autonName, void(*function)()) {
  Autonomous.insert(make_pair(slot, function));
  SlotName.insert(make_pair(slot, autonName));
}

void setAuton(int slot) {
  autonSlot = slot;
}

const char * getName(int slot) {
  return SlotName[slot];
}
