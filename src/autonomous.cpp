#include "main.h"

#include "control/asyncController.h"
#include "control/autonController.h"
#include "control/path.h"
using namespace okapi;
using namespace path;
using namespace std;

static ControlAsync Control;

bool Autonomous::isInitialized = false;
int Autonomous::type = SLOT_RED,
Autonomous::slot = 2;

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

    // Red
    addAuton(SLOT_RED, "Small zone 8 cubes", r_s_8); // Slot 0
    addAuton(SLOT_RED, "Big zone 8 cubes", r_b_8);
    addAuton(SLOT_RED, "Big zone orange tower", r_b_orange);
    // Blue
    addAuton(SLOT_BLUE, "Small zone 8 cubes", b_s_8); // Slot 0
    addAuton(SLOT_BLUE, "Big zone 8 cubes", b_b_8);
    addAuton(SLOT_BLUE, "Big zone green tower", b_b_green);

    // Skills
    addAuton(SLOT_SKILLS, "Tester", tester); // Slot 0
    addAuton(SLOT_SKILLS, "Official Skills 1", skills1);

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

void Autonomous::addAuton(int type_, std::string autonName, void(*function)()) {
  switch(type_) {
    case SLOT_RED: {
      redAuton.insert(make_pair(redAuton.size(), function));
      slotName[redAuton.size() - 1].red = autonName;
      break;
    }

    case SLOT_BLUE: {
      blueAuton.insert(make_pair(blueAuton.size(), function));
      slotName[blueAuton.size() - 1].blue = autonName;
      break;
    }

    case SLOT_SKILLS: {
      skillsAuton.insert(make_pair(skillsAuton.size(), function));
      slotName[skillsAuton.size() - 1].skills = autonName;
      break;
    }

    default: {
      cout << "Auton name " << autonName << " not registered" << endl;
      break;
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
  return type;
}

void Autonomous::setType(int type_) {
  type = type_;
}

int Autonomous::getSize(int type_) {
  switch(type_) {
    case 1: { return redAuton.size(); break; }
    case 2: { return blueAuton.size(); break; }
    case 3: { return skillsAuton.size(); break; }
    case 4: { return slotName.size(); break; }

    default: { return slotName.size(); break; }
  }
}

std::string Autonomous::getName(int type_, int slot_) {
  switch(type_) {
    case SLOT_RED: {
      return slotName[slot_].red;
      break;
    }

    case SLOT_BLUE: {
      return slotName[slot_].blue;
      break;
    }

    case SLOT_SKILLS: {
      return slotName[slot_].skills;
      break;
    }

    default: {
      return "Incorrect Type";
      break;
    }
  }
}
