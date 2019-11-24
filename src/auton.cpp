#include "main.h"

#include "controller/chassis.h"
#include "controller/rack.h"
#include "controller/arm.h"

#include "controller/misc.h"
#include "controller/vision.h"

static Chassis chassis;
static Rack rack;
static Arm arm;
static Camera CamFront(FRONTVISION);

using namespace io;

// Ignore below. Just for testing stuff.
void tester() {
  std::cout << "Testing" << std::endl;
  chassis.drive(5, 127).waitUntilSettled();
}

/*===========================================
  PREMADE FUNCTIONS
===========================================*/

void deploy() {
  arm.move(ARM_LOW_TOWER, 127).waitUntilSettled();
  roller(-0.2,200);
  arm.zero().waitUntilSettled();
}

/*===========================================
  RED MATCH AUTONOMOUSES
===========================================*/
void red1() {
  chassis.calibrateGyro();
  delay(700);
  deploy();
  roller(127);
  chassis.drive(200, 80).waitUntilSettled();

  chassis.turn(100, 80).waitUntilSettled();
  chassis.withGyro(-90, 2).drive(200, 40).waitUntilSettled();
}

/*===========================================
  BLUE MATCH AUTONOMOUSES
===========================================*/
void blue1() {

}

/*===========================================
  SKILLS AUTONOMOUSES
===========================================*/
void skills1() {

}
