#include "main.h"

#include "kari/control/chassis.h"
#include "kari/control/rack.h"
#include "kari/control/arm.h"

#include "kari/util/misc.h"
#include "kari/autonController.h"
#include "kari/displayController.h"

using namespace okapi;
using namespace std;

bool Autonomous::isInitialized = false;
int Autonomous::type = SLOT_RED,
Autonomous::slot = 1;

std::map<int, void(*)()> Autonomous::redAuton;
std::map<int, void(*)()> Autonomous::blueAuton;
std::map<int, void(*)()> Autonomous::skillsAuton;
std::map<int, typeName> Autonomous::slotName;
std::map<int, typeName> Autonomous::slotAbbv;

void autonomous() {
  Chassis chassis; chassis.reset();
  Rack rack; rack.reset();
  Arm arm; arm.reset();

  Autonomous Auton;
  Auton.runAuton();
}

Autonomous::Autonomous() { // The autons will be stored in this order, starting from 0.
  if(!isInitialized) {

    // RED
    addAuton(SLOT_RED, "Red small zone 11", "RSZns", redsmallzone11);
    //addAuton(SLOT_RED, "Red small zone8", "RSZ", redsmallzone8);
    addAuton(SLOT_RED, "Red small zone7", "RSZU", redsmallzone7);
    addAuton(SLOT_RED, "Red small zone5", "RSZ", redsmallzone5);
    addAuton(SLOT_RED, "Red big zone", "RBZ", redbigzone);
    addAuton(SLOT_RED, "Red big zone score", "RBZs", redbigzonescore);

    // BLUE
    addAuton(SLOT_BLUE, "blue small zone 11", "RSZns", bluesmallzone11);
    //addAuton(SLOT_BLUE, "blue small zone9", "BSZ", bluesmallzone9);
    addAuton(SLOT_BLUE, "blue small zone7", "BSZ", bluesmallzone7);
    addAuton(SLOT_BLUE, "blue small zone5", "BSZ", bluesmallzone5);
    addAuton(SLOT_BLUE, "blue big zone", "BBZ", bluebigzone);
    addAuton(SLOT_BLUE, "blue big zone score", "BBZs", bluebigzonescore);

    // Skills
    addAuton(SLOT_SKILLS, "Tester", "Tester", tester); // Slot 0
    addAuton(SLOT_SKILLS, "Skills 1", "Skills1", skills1);
    addAuton(SLOT_SKILLS, "Skills 2", "Skills2", skills2);
    //addAuton(SLOT_SKILLS, "Skills 3", "Skills3", skills3);



    isInitialized = true;
  }
}

void Autonomous::runAuton() {
  switch(type) {
    case SLOT_RED: redAuton[slot](); break;
    case SLOT_BLUE: blueAuton[slot](); break;
    case SLOT_SKILLS: skillsAuton[slot](); break;

    default: cout << "Auton not selected" << endl; break;
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

void Autonomous::start(void* ignore) {
  pros::delay(500);
  Autonomous* that = static_cast<Autonomous*>(ignore);
  that -> run();
}

void Autonomous::run() {
  Display display;

  while(true) {
    display.setRemoteText(getName(getType(), getSlot()));
    pros::delay(50);
  }
}
