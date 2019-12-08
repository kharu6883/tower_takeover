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

/*===========================================
  DICTIONARY
===========================================*/
// IDLE = 0
// DRIVING = 1
// TURNING = 2
// ALIGNING = 3

// Ignore below. Just for testing stuff.
void tester() {
  std::cout << "Testing" << std::endl;
  chassis.drive(1000, 127);
  delay(100);
  chassis.clearArr();
  chassis.setMode(IDLE);
  chassis.withTarget(-500, 100, 0).drive().waitUntilSettled();
  io::master.rumble(" .");
}

/*===========================================
  PREMADE FUNCTIONS
===========================================*/

void deploy(double a) {
  //arm.move(0.2, 127).waitUntilSettled();
  roller(-a,127);
  //arm.zero().waitUntilSettled();
}

/*===========================================
  RED MATCH AUTONOMOUSES
===========================================*/
void red1() {
  chassis.calibrateGyro();
   delay(500);
   deploy(0.7);
   delay(200);
   roller(127);
   chassis.drive(2300, 50,7).withTol(5).waitUntilSettled();
   chassis.withGyro(-45,1).drive(-1900, 127,10).waitUntilSettled();
   chassis.withGyro(0,2).drive(1300, 70,1).waitUntilSettled();
   roller(127);
   chassis.turn(550, 80, 5).waitUntilSettled();
   chassis.unlock();
   chassis.withGyro(-131,0.7).drive(2200, 100,1);
   delay(500);
   roller(-0.3,70);
   delay(1500);
   rack.move(RACK_UP,127,13).withTol(10).waitUntilSettled();
   roller(-100);
   delay(50);
   chassis.drive(-500, 127,5).withTol(5).waitUntilSettled();

   io::master.rumble(" .");
}

void redbigzone() {
   chassis.calibrateGyro();
   delay(400);
   arm.move(0.5, 127);
   delay(300);
   deploy(0.6);
   arm.zero();
   roller(127);
   delay(200);
   chassis.drive(300, 100).waitUntilSettled();
   chassis.withTarget(700, 50, -85, 1, 1).withTarget(1600, 60, -25,2 ,3).withTarget(2600, 60, 90, 1).drive().waitUntilSettled();
   chassis.withTarget(-1600, 100, 25, 3, 3).withTarget(-200, 100, 90,2 ,3).withTarget(900, 100, 105, 1).drive().waitUntilSettled();
   roller(-0.5,60);
   rack.move(RACK_UP,127,13).withTol(20).waitUntilSettled();
   roller(-100);
   rack.move(RACK_DOWN,127,13).withTol(10);
   chassis.drive(-500, 127,5).withTol(5).waitUntilSettled();


}

/*===========================================
  BLUE MATCH AUTONOMOUSES
===========================================*/
void blue1() {

  chassis.calibrateGyro();
   delay(500);
   deploy(0.7);
   delay(200);
   roller(127);
   chassis.drive(2300, 50,7).withTol(5).waitUntilSettled();
   chassis.withGyro(45,1).drive(-1900, 127,10).waitUntilSettled();
   chassis.withGyro(0,2).drive(1300, 70,1).waitUntilSettled();
   roller(127);
   chassis.turn(-550, 80, 5).waitUntilSettled();
   chassis.unlock();
   chassis.withGyro(131,0.7).drive(2200, 100,1);
   delay(500);
   roller(-0.3,70);
   delay(1500);
   rack.move(RACK_UP,127,13).withTol(10).waitUntilSettled();
   roller(-100);
   delay(50);
   chassis.drive(-500, 127,5).withTol(5).waitUntilSettled();

   delay(10000);




    chassis.calibrateGyro();
     delay(500);
     deploy(0.7);
     delay(200);
     roller(127);
     chassis.drive(2300, 50,7).withTol(5).waitUntilSettled();
     chassis.withGyro(45,1).drive(-1900, 127,10).waitUntilSettled();
     chassis.withGyro(0,2).drive(1300, 70,1).waitUntilSettled();
     roller(127);
     chassis.turn(-550, 80, 5).waitUntilSettled();
     chassis.unlock();
     chassis.withGyro(131,0.7).drive(2100, 100,1).waitUntilSettled();
     roller(-0.3,70);
     rack.move(RACK_UP,127,13).withTol(10).waitUntilSettled();
     roller(-100);
     chassis.drive(-500, 127,5).withTol(5).waitUntilSettled();

     io::master.rumble(" .");
}

void bluebigzone() {

  chassis.calibrateGyro();
  delay(400);
  arm.move(0.5, 127);
  delay(300);
  deploy(0.6);
  arm.zero();
  roller(127);
  delay(200);
  chassis.drive(300, 100).waitUntilSettled();
  chassis.withTarget(700, 50, 85, 1, 1).withTarget(1600, 60, 25,2 ,3).withTarget(2600, 60, -90, 1).drive().waitUntilSettled();
  chassis.withTarget(-1600, 100, -25, 3, 3).withTarget(-200, 100, -90,2 ,3).withTarget(900, 100, -105, 1).drive().waitUntilSettled();
  roller(-0.5,60);
  rack.move(RACK_UP,127,13).withTol(20).waitUntilSettled();
  roller(-100);
  rack.move(RACK_DOWN,127,13).withTol(10);
  chassis.drive(-500, 127,5).withTol(5).waitUntilSettled();

}

/*===========================================
  SKILLS AUTONOMOUSES
===========================================*/
void skills1() {
  chassis.calibrateGyro();
  delay(500);
  arm.move(1.05, 127);
  chassis.lock();
  chassis.withGyro(-50, 1).drive(1250, 70, 3).waitUntilSettled();
  //deploy, drive to tower

  arm.waitUntilSettled();
  roller(-60);
  delay(600);
  //score red tower

  roller(127);
  chassis.withGyro(-15, 0.5).drive(-900, 100, 9);
  arm.zero().waitUntilSettled();
  chassis.waitUntilSettled();
  delay(300);
  chassis.unlock();
  chassis.waitUntilSettled();

  chassis.withTarget(1500, 60, 0, 1.3,1).withTarget(400, 50, -35, 0.5).withTarget(600, 127, 0, 1.3).withTarget(600, 80, 35, 0.5).withTarget(2650, 50, 0, 1.3).withTarget(900, 80, -45, 1.3).drive().waitUntilSettled();
  roller(100);
  chassis.left(80);
  chassis.right(80);
  rack.move(RACK_UP,127,13).withTol(20);
  delay(500);
  chassis.left(0);
  chassis.right(0);
  rack.waitUntilSettled();
  roller(-100);
  rack.move(RACK_DOWN,127,13).withTol(10);
  chassis.drive(-400, 100,8).withTol(5).waitUntilSettled();
  roller(0);
  chassis.turn(150, 100,8).withTol(5).waitUntilSettled();
  arm.move(0.7, 127);
  chassis.withGyro(-85, 2).drive(-1500, 70, 3).waitUntilSettled();
  chassis.turn(400, 100,8).waitUntilSettled();
  chassis.left(-80);
  chassis.right(-80);
  delay(500);
  chassis.left(0);
  chassis.right(0);
  roller(0);
  chassis.calibrateGyro();
  delay(500);
  arm.zero().waitUntilSettled();
  roller(127);
  chassis.lock();
  chassis.withGyro(0,2).drive(2700, 80, 6).waitUntilSettled();
  delay(300);
  arm.withTol(20).tower(1);
  delay(800);
  chassis.withGyro(45,1).withTol(10).drive(350, 40, 3).waitUntilSettled();
  roller(-80);
  delay(1000);
  roller(0);
  chassis.withTarget(-600, 100,0,0.4,9).drive();
  delay(700);
  arm.zero();
  roller(127);
  chassis.waitUntilSettled();
  arm.waitUntilSettled();
  chassis.withGyro(0,2).drive(4000, 60, 1).waitUntilSettled();
  delay(500);
  chassis.withGyro(0,2).drive(-1400, 60, 1).waitUntilSettled();
  roller(127);
  chassis.turn(-200, 127,5).withTol(10).withConst(0.6).waitUntilSettled();
  chassis.withGyro(45,2).drive(2300, 127, 5).waitUntilSettled();
  roller(-0.3,60);
  chassis.left(40);
  chassis.right(40);
  rack.move(RACK_UP,127,13).withTol(20).waitUntilSettled();
  roller(-100);
  rack.move(RACK_DOWN,127,13).withTol(10);
  chassis.withGyro(45,2).drive(-2000, 127, 5).waitUntilSettled();
  roller(127);
  chassis.turn(600, 127,5).withTol(10).waitUntilSettled();
  chassis.withGyro(-80,2).drive(5000, 80, 3).waitUntilSettled();
  chassis.withGyro(-80,2).drive(-1000, 80, 3).waitUntilSettled();
  arm.tower(1);
  delay(500);
  arm.move(1.05, 127);
  chassis.turn(400, 127,5).withTol(10).waitUntilSettled();
  chassis.drive(400, 80, 3).waitUntilSettled();
  roller(-90);
  delay(400);
  chassis.drive(-1100, 80, 3).waitUntilSettled();
  arm.zero();
  roller(127);
  chassis.turn(-700, 127,5).withTol(10).waitUntilSettled();
  chassis.withGyro(-45).drive(1500, 80, 3).waitUntilSettled();
  roller(-0.3,60);
  chassis.left(60);
  chassis.right(60);
  rack.move(RACK_UP,127,13).withTol(20).waitUntilSettled();
  roller(-100);
  rack.move(RACK_DOWN,127,13).withTol(10);
  chassis.drive(-700, 127, 5).waitUntilSettled();
  chassis.turn(-800, 127,5).withTol(10).waitUntilSettled();
  chassis.left(-40);
  chassis.right(-40);
  delay(1000);
  roller(127);
  chassis.left(0);
  chassis.right(0);

}
