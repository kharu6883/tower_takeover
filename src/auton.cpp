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
  //arm.move(0.2, 127).waitUntilSettled();
  roller(-0.4,200);
  //arm.zero().waitUntilSettled();
}

/*===========================================
  RED MATCH AUTONOMOUSES
===========================================*/
void red1() {
  chassis.calibrateGyro();
   delay(500);
   deploy();
   delay(200);
   roller(127);
   chassis.drive(2300, 50).withTol(5).waitUntilSettled();
   chassis.withGyro(-45,1.1).drive(-1900, 127,10).waitUntilSettled();
   chassis.withGyro(0,2).drive(1100, 70,1).waitUntilSettled();
   roller(127);
   chassis.withGyro(-131,0.5).drive(2500, 100,1).waitUntilSettled();
   roller(-0.3,70);
   rack.move(RACK_UP,127,13).withTol(10).waitUntilSettled();
   roller(-100);
   chassis.drive(-500, 127,5).withTol(5).waitUntilSettled();

   io::master.rumble(" .");
}

/*===========================================
  BLUE MATCH AUTONOMOUSES
===========================================*/
void blue1() {
  chassis.calibrateGyro();
   delay(500);
   deploy();
   roller(127);
   chassis.drive(2300, 70).withTol(5).waitUntilSettled();
   chassis.withGyro(45,0.5).drive(-1800, 100,8).waitUntilSettled();
   chassis.withGyro(0,3).drive(1200, 40,1).waitUntilSettled();
   chassis.withGyro(135,2).drive(2400, 70,5).waitUntilSettled();
   roller(-0.2,100);
   rack.move(RACK_UP,127,13).withTol(10).waitUntilSettled();
   roller(-127);
   chassis.drive(-500, 70,15).withTol(5).waitUntilSettled();

   io::master.rumble(" .");
}

/*===========================================
  SKILLS AUTONOMOUSES
===========================================*/
void skills1() {

}
