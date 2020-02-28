#include "main.h"

#include "kari/control/chassis.h"
#include "kari/control/rack.h"
#include "kari/control/arm.h"

#include "kari/util/odometry.h"
#include "kari/util/misc.h"
#include "kari/util/vision.h"

static Chassis chassis;
static Rack rack;
static Arm arm;
static Odom odom;

using namespace io;

// Make Trigger

/*===========================================
  DICTIONARY
===========================================*/
// IDLE = 0
// DRIVING_POINT = 1
// DRIVING_DIST = 2
// TURNING = 3
// STRAFING = 4

void tester() {
  chassis.withGain(0.25, 0.5, 0.5).withTurnGain(2, 0.3, 3.3).withTol(25);
  chassis.withTarget(1000, 0, 100, 4)
          .withTarget(2000, 45, 100, 4).drive().waitUntilSettled();
  // chassis.turn(90,60,5).withTol(5).waitUntilSettled();
  io::master.rumble(" . .");
}

/*===========================================
  PREMADE FUNCTIONS
===========================================*/
void deploy() {
  arm.move(ARM_LOW_TOWER, 127).withTol(0.3).waitUntilSettled();
  arm.zero();
}

/*===========================================
  RED MATCH AUTONOMOUSES
===========================================*/
void redsmallzone11() {

}

void redsmallzone8() {

}

void redsmallzone7() {

}

void redsmallzone5() {

}

void redbigzone() {

}

void redbigzonescore() {

}

/*===========================================
  BLUE MATCH AUTONOMOUSES
===========================================*/
void bluesmallzone11() {

}

void bluesmallzone8() {

}

void bluesmallzone7() {

}

void bluesmallzone5() {

}

void bluebigzone() {

}

void bluebigzonescore() {

}

/*===========================================
  SKILLS AUTONOMOUSES
===========================================*/
void skills1() {

}

void skills2() {

}

void skills3() {

}
