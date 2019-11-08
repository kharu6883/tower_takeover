#include "main.h"

#include "controller/autonController.h"
#include "config/io.h"
using namespace okapi;
using namespace std;

bool Autonomous::isInitialized = false;
int Autonomous::type = SLOT_RED,
Autonomous::slot = 0;

std::map<int, void(*)()> Autonomous::redAuton;
std::map<int, void(*)()> Autonomous::blueAuton;
std::map<int, void(*)()> Autonomous::skillsAuton;
std::map<int, typeName> Autonomous::slotName;
std::map<int, typeName> Autonomous::slotAbbv;

void autonomous() {
  Autonomous Auton;
  Auton.runAuton();
}

Autonomous::Autonomous() { // The autons will be stored in this order, starting from 0.
  if(!isInitialized) {

    // Red
    addAuton(SLOT_RED, "Small zone 7 cubes", "Red S 7", r_s_7);
    addAuton(SLOT_RED, "Small zone 8 cubes", "Red S 8", r_s_8); // Slot 0
    addAuton(SLOT_RED, "No place 9 cubes", "Red No Cube 9", r_n_9);
    addAuton(SLOT_RED, "Big zone orange tower", "Red B O", r_b_orange);

    // Blue
    addAuton(SLOT_BLUE, "Small zone 7 cubes", "Blue S 7", b_s_7);
    addAuton(SLOT_BLUE, "Small zone 8 cubes", "Blue S 8", b_s_8); // Slot 0
    addAuton(SLOT_BLUE, "No place 9 cubes", "Blue No Cube 9", b_n_9);
    addAuton(SLOT_BLUE, "Big zone green tower", "Blue B G", b_b_green);

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

void Autonomous::run() {
  bool isPressed, isReleased, isHeld;
  int dT;

  okapi::Timer timer;

  while(true) {
    if(selector.get_value()) isPressed = true;
      else isPressed = false;

    if(selector.get_value()) {
			int last = timer.millis().convert(okapi::millisecond);
			while(selector.get_value()) pros::delay(20);
			int now = timer.millis().convert(okapi::millisecond);
			isReleased = true;
			dT = now - last;
		}

    if(dT > 500) {
      isHeld = true;
      dT = 0;
    }

    if(isReleased && !isHeld) {
      slot++;
      isReleased = false;
    } else if(isReleased && isHeld) {
      type++;
      isReleased = false;
      isHeld = false;
    }

    if(type > SLOT_SKILLS) type = SLOT_RED;
    if(slot >= getSize(getType())) slot = 0;

    pros::delay(20);
  }
}

void Autonomous::start(void* ignore) {
  pros::delay(500);
  Autonomous* that = static_cast<Autonomous*>(ignore);
  that -> run();
}
