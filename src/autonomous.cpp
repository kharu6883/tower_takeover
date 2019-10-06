#include "main.h"

#include "control/asyncController.h"
#include "control/autonController.h"
#include "control/path.h"
using namespace okapi;
using namespace path;
using namespace std;

static ControlAsync Control;

bool Autonomous::isInitialized = false;
int Autonomous::slot = 1,
Autonomous::type = SLOT_RED;

std::map<int, void(*)()> Autonomous::redAuton;
std::map<int, void(*)()> Autonomous::blueAuton;
std::map<int, void(*)()> Autonomous::skillsAuton;
std::map<int, typeName> Autonomous::slotName;

void autonomous() {
  Control.resume();
  Autonomous Auton;
  Auton.runAuton();
}

Autonomous::Autonomous() { // The autons will be stored in this order, starting from 0.
  if(!isInitialized) {
    addAuton(SLOT_SKILLS, "Test Auton", tester); // Slot 0

    // Red
    addAuton(SLOT_RED, "Red Small zone 8 cubes", r_s_8);
    addAuton(SLOT_RED, "Red Big zone 8 cubes", r_b_8);

    // Blue
    addAuton(SLOT_BLUE, "Blue Small zone 8 cubes", b_s_8);
    addAuton(SLOT_BLUE, "Blue Big zone 8 cubes", b_b_8);

    // Skills
    addAuton(SLOT_SKILLS, "Skills 1", skills1);

    isInitialized = true;
  }
}

void Autonomous::runAuton() {
  switch(type) {
    case SLOT_RED: redAuton[slot]();
    case SLOT_BLUE: blueAuton[slot]();
    case SLOT_SKILLS: skillsAuton[slot]();

    default: print("Slot Not Selected");
  }
}

void Autonomous::addAuton(int type_, const char * autonName, void(*function)()) {
  switch(type_) {
    case SLOT_RED: {
      typeName name = {
        autonName,
        slotName[redAuton.size()].blue,
        slotName[redAuton.size()].skills
      };

      redAuton.insert(make_pair(redAuton.size(), function));
      slotName.insert(make_pair(redAuton.size(), name));
    }

    case SLOT_BLUE: {
      typeName name = {
        slotName[blueAuton.size()].red,
        autonName,
        slotName[blueAuton.size()].skills
      };

      blueAuton.insert(make_pair(blueAuton.size(), function));
      slotName.insert(make_pair(blueAuton.size(), name));
    }

    case SLOT_SKILLS: {
      typeName name = {
        slotName[skillsAuton.size()].red,
        slotName[skillsAuton.size()].blue,
        autonName
      };

      skillsAuton.insert(make_pair(skillsAuton.size(), function));
      slotName.insert(make_pair(skillsAuton.size(), name));
    }

    default: {

    }
  }
}



int Autonomous::getSlot() {
  return slot;
}

void Autonomous::setSlot(int slot_) {
  slot = slot_;
}

int Autonomous::getType() {
  return slot;
}

void Autonomous::setType(int type_) {
  type = type_;
}

int Autonomous::getSize() {
  return slotName.size();
}

const char * Autonomous::getName(int type, int slot) {
  switch(type) {
    case SLOT_RED: return slotName[slot].red;
    case SLOT_BLUE: return slotName[slot].blue;
    case SLOT_SKILLS: return slotName[slot].skills;

    default: return "Incorrect Type";
  }
}
