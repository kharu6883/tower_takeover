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

  chassis.calibrateGyro();
  delay(500);
   arm.move(1.05, 127);
  chassis.withGyro(-50, 1).drive(1100, 70, 3).waitUntilSettled();
  arm.waitUntilSettled();
  roller(-60);
   delay(600);

   roller(127);
chassis.withGyro(-35, 0.5).drive(-900, 100, 6).waitUntilSettled();
  arm.zero();
  delay(300);
  chassis.waitUntilSettled();
  chassis.withTarget(1700, 60, 0, 1.3,1).withTarget(400, 50, -35, 0.5).withTarget(400, 127, 0, 1.3).withTarget(600, 80, 35, 0.5).withTarget(2650, 50, 0, 1.3).withTarget(900, 80, -40, 1.3).drive().waitUntilSettled();
  roller(100);
  rack.move(RACK_UP,127,13).withTol(10).waitUntilSettled();
  roller(-120);
  rack.move(RACK_DOWN,127,13).withTol(10);
  chassis.drive(-300, 127,8).withTol(5).waitUntilSettled();
  chassis.turn(200, 127,8).withTol(5).waitUntilSettled();
  chassis.drive(-1400, 127,15).withTol(5).waitUntilSettled();
  chassis.turn(400, 127,8).waitUntilSettled();
  chassis.left(-80);
  chassis.right(-80);
  delay(1100);
  chassis.left(0);
  chassis.right(0);
  roller(0);
  chassis.calibrateGyro();
  delay(500);
  roller(127);
  chassis.drive(2400, 80, 6).waitUntilSettled();
  delay(300);
  arm.withTol(20).tower(1);
  delay(300);
  chassis.withGyro(55,1).withTol(10).drive(500, 40, 3).waitUntilSettled();
  roller(-80);
  delay(1000);
  roller(0);
  chassis.withGyro(120,3).drive(1800, 100, 1);
  delay(400);
  arm.tower(1);
  chassis.waitUntilSettled();
  arm.waitUntilSettled();
  roller(-80);
  delay(1000);
  chassis.withTarget(-1300, 100,110,1,6).withTarget(-500, 100,0,1,6).drive();
  delay(300);
  arm.zero();
  roller(127);
  chassis.waitUntilSettled();
  arm.waitUntilSettled();
  chassis.withGyro(0,2).drive(4000, 60, 1).waitUntilSettled();
  chassis.withGyro(0,2).drive(-1000, 60, 1).waitUntilSettled();
  arm.tower(2);
  chassis.turn(300, 127,1).withTol(10).waitUntilSettled();
  roller(-127);
  delay(1000);
  roller(127);
  chassis.turn(600, 127,5).withTol(10).waitUntilSettled();
  arm.zero();
  chassis.left(-100);
  chassis.right(-100);
  delay(800);
  chassis.left(0);
  chassis.right(0);
  chassis.calibrateGyro();
  delay(500);
  roller(127);
  chassis.withTarget(1900, 50, 0, 1,4).withTarget(2200, 50, 95, 1,4).withTarget(3000, 70, 140, 1,4).drive().waitUntilSettled();//withTarget(400, 50, -35, 0.5).withTarget(400, 127, 0, 1.3).withTarget(600, 80, 35, 0.5).withTarget(2650, 50, 0, 1.3).withTarget(900, 80, -40, 1.3).drive().waitUntilSettled();

  rack.move(RACK_UP,127,13).withTol(10).waitUntilSettled();
  roller(100);
  delay(300);
  roller(-120);
  rack.move(RACK_DOWN,127,13).withTol(10);
  chassis.drive(-500, 127,8).withTol(5).waitUntilSettled();
}
