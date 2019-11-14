#include "main.h"

#include "config/io.h"
#include "config/motor.h"

#include "control/displayController.h"
#include "control/autonController.h"
#include "control/asyncController.h"
#include "control/macro.h"
#include "control/path.h"
using namespace Display;

static ControlAsync Control;
pros::Task armAsync(macroTask, NULL, "Opcontrol Arm Task");

void autonSelector(void* args) {
  bool isPressed, isReleased, isHeld;
  int dT;

  Autonomous auton;
  BrainDisplay display;

  int type = auton.getType(), slot = auton.getSlot(), limit = auton.getSize(type);
  int lastType, lastSlot;

  okapi::Timer timer;

  lastType = type;
  lastSlot = slot;

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

    limit = auton.getSize(type);

    if(type > 3) type = 1;
    if(slot >= limit) slot = 0;

    if(type != lastType || slot != lastSlot) {
      auton.setType(type);
      auton.setSlot(slot);
    }

    lastType = type;
    lastSlot = slot;

    pros::delay(20);
  }
}

void initialize() {
  print("Starting Init Routine");

  Autonomous Auton;
  print("Auton Set!");

  armAsync.suspend();

  // Motor Initialization
  Arm.tare_position();

  Rack.set_brake_mode(MOTOR_BRAKE_HOLD);
  Arm.set_brake_mode(MOTOR_BRAKE_HOLD);

  RollerL.set_brake_mode(MOTOR_BRAKE_HOLD);
	RollerR.set_brake_mode(MOTOR_BRAKE_HOLD);
  print("Motors Initialized!");

  // Threads
  pros::Task asyncController(Control.run, NULL, "Async Controller");
  Control.pause();
  print("Task Controller Initialized!");

  BrainDisplay Brain;
  Brain.main();
  pros::Task b_display(Brain.run, NULL, "Brain Display");
  b_display.set_priority(TASK_PRIORITY_MIN);
  print("Display Initialized!");

  RemoteDisplay Remote;
  pros::Task r_display(Remote.run, NULL, "Remote Display");
  r_display.set_priority(TASK_PRIORITY_MIN);
  print("Remote Initialized!");

  Path path;
  pros::Task pathMaker(path.start, NULL, "Path Maker");
  pathMaker.set_priority(TASK_PRIORITY_MIN);
  print("Pathmaker Initialized!");

  pros::Task autonSelect(autonSelector, NULL, "Auton Selector");
  autonSelect.set_priority(TASK_PRIORITY_MIN);
  print("Auton Selector Initialized!");

  std::cout << "Initialization Done!" << std::endl;
}

void disabled() {
  Control.pause();
  armAsync.suspend();
}

void competition_initialize() { }
