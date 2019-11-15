#include "main.h"

#include "control/asyncController.h"
#include "control/autonController.h"
using namespace okapi;
using namespace std;

static ControlAsync Control;

bool Autonomous::isInitialized = false;
int Autonomous::type = SLOT_SKILLS,
Autonomous::slot = 0;

std::map<int, void(*)()> Autonomous::redAuton;
std::map<int, void(*)()> Autonomous::blueAuton;
std::map<int, void(*)()> Autonomous::skillsAuton;
std::map<int, typeName> Autonomous::slotName;
std::map<int, typeName> Autonomous::slotAbbv;

void autonomous() {
  Control.resume();
  Autonomous Auton;
  Auton.runAuton();
}

Autonomous::Autonomous() { // The autons will be stored in this order, starting from 0.
  if(!isInitialized) {

    // Red
    addAuton(SLOT_RED, "Small zone 7 cubes", "Red S 7", r_s_7);
    addAuton(SLOT_RED, "RED No place 9 cubes", "Red B 9", r_b_9);

    // Blue
    addAuton(SLOT_BLUE, "Small zone 7 cubes", "Blue S 7", b_s_7);
    addAuton(SLOT_BLUE, "BLUE No place 9 cubes", "Blue B 9", b_b_9);

    // Skills
    addAuton(SLOT_SKILLS, "Tester", "Tester", tester); // Slot 0
    addAuton(SLOT_SKILLS, "Official Skills 1", "Skills", skills1);

    isInitialized = true;
  }
}

void Autonomous::runAuton() {
  switch(type) {
    case SLOT_RED: redAuton[slot](); break;
    case SLOT_BLUE: blueAuton[slot](); break;
    case SLOT_SKILLS: skillsAuton[slot](); break;

    default: print("Slot Not Selected"); break;
  }
}

void Autonomous::addAuton(int type_, std::string autonName, std::string autonAbbv, void(*function)()) {
  switch(type_) {
    case SLOT_RED: {
      redAuton.insert(make_pair(redAuton.size(), function));
      slotName[redAuton.size() - 1].red = autonName;
      slotAbbv[redAuton.size() - 1].red = autonAbbv;
      break;
    }

    case SLOT_BLUE: {
      blueAuton.insert(make_pair(blueAuton.size(), function));
      slotName[blueAuton.size() - 1].blue = autonName;
      slotAbbv[blueAuton.size() - 1].blue = autonAbbv;
      break;
    }

    case SLOT_SKILLS: {
      skillsAuton.insert(make_pair(skillsAuton.size(), function));
      slotName[skillsAuton.size() - 1].skills = autonName;
      slotAbbv[skillsAuton.size() - 1].skills = autonAbbv;
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

std::string Autonomous::getAbbv(int type_, int slot_) {
  switch(type_) {
    case SLOT_RED: {
      return slotAbbv[slot_].red;
      break;
    }

    case SLOT_BLUE: {
      return slotAbbv[slot_].blue;
      break;
    }

    case SLOT_SKILLS: {
      return slotAbbv[slot_].skills;
      break;
    }

    default: {
      return "Incorrect Type";
      break;
    }
  }
}
