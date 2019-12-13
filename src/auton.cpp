#include "main.h"

#include "lib_7k/control/chassis.h"
#include "lib_7k/control/rack.h"
#include "lib_7k/control/arm.h"

#include "lib_7k/util/misc.h"
#include "lib_7k/util/vision.h"

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

void deploy(double a) {
  //arm.move(0.2, 127).waitUntilSettled();
  roller(-a,200);
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
   chassis.withGyro(-131,0.7).drive(2100, 100,1).waitUntilSettled();
   roller(-0.3,70);
   rack.move(RACK_UP,127,13).withTol(10).waitUntilSettled();
   roller(-100);
   chassis.drive(-500, 127,5).withTol(5).waitUntilSettled();

   io::master.rumble(" .");
}

void redbigzone() {
   chassis.calibrateGyro();
   delay(500);
   deploy(0.4);
   delay(300);
   roller(127);
   chassis.drive(300, 100).waitUntilSettled();
   chassis.withTarget(700, 50, -85, 1, 1).withTarget(1600, 60, -25,2 ,3).withTarget(2600, 60, 90, 1).drive().waitUntilSettled();
   chassis.withTarget(-1600, 100, 25, 3, 3).withTarget(-200, 100, 90,2 ,3).withTarget(800, 100, 105, 1).drive().waitUntilSettled();
   roller(-0.5,100);
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
     chassis.withGyro(131,0.7).drive(2100, 100,1).waitUntilSettled();
     roller(-0.3,70);
     rack.move(RACK_UP,127,13).withTol(10).waitUntilSettled();
     roller(-100);
     chassis.drive(-500, 127,5).withTol(5).waitUntilSettled();

     io::master.rumble(" .");
}

void bluebigzone() {
  chassis.calibrateGyro();
  arm.withTol(30).move(1.15, 127);
  delay(800);
  deploy(0.4);
  arm.zero().waitUntilSettled();
  roller(127);
  chassis.withGyro(0,2).drive(400,80,4).waitUntilSettled();
  chassis.turn(-300, 80, 5);
  chassis.withGyro(85,1).drive(700,50,2).waitUntilSettled();
  chassis.withGyro(25,2).drive(1800,60,3).waitUntilSettled();
  chassis.withGyro(-100,1).drive(1000,70,3).waitUntilSettled();
  chassis.withGyro(-90,1).drive(1200,100,8).waitUntilSettled();


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
  chassis.withGyro(-40, 0.5).drive(-1150, 100, 6).waitUntilSettled();
  arm.zero();
  delay(300);
  chassis.unlock();
  chassis.waitUntilSettled();
  //re align to row of 9 cubes

  chassis.withTarget(1700, 60, 0, 1.6,1).withTarget(400, 50, -35, 0.5).withTarget(400, 127, 0, 1.3).withTarget(600, 80, 35, 0.5).withTarget(2650, 50, 0, 1.3).withTarget(900, 127, -50, 1.3).drive().waitUntilSettled();
  //multipoint drive to blue small corner with 9 cubes

  roller(100);
  rack.move(RACK_UP,127,13).withTol(10).waitUntilSettled();
  roller(-120);
  rack.move(RACK_DOWN,127,13).withTol(10);
  //place tower of 9 cubes

  chassis.drive(-300, 127,8).withTol(5).waitUntilSettled();
  chassis.turn(150, 127,8).withTol(5).waitUntilSettled();
  chassis.drive(-1300, 127,15).withTol(5).waitUntilSettled();
  chassis.turn(350, 127,8).waitUntilSettled();
  chassis.left(-80);
  chassis.right(-80);
  delay(1100);
  chassis.left(0);
  chassis.right(0);
  roller(0);
  chassis.calibrateGyro();
  //back away from scoring zone and recalibrate the gyroscope

  delay(500);
  roller(127);
  chassis.drive(2300, 80, 6).waitUntilSettled();
  delay(300);
  arm.withTol(20).tower(1);
  delay(1000);
  arm.move(1.1,127);
  chassis.withGyro(55,1).withTol(10).drive(150, 40, 3).waitUntilSettled();
  roller(-80);
  //score center low cup and grab 3 cubes

  delay(1000);
  chassis.withGyro(20,1).withTol(10).drive(-300, 30, 9);
  delay(400);
  arm.zero();
  roller(127);
  chassis.waitUntilSettled();
  arm.waitUntilSettled();
  //realign to part two of row of three cubes

  chassis.withGyro(0,2).drive(3500, 60, 1).waitUntilSettled();
  chassis.withGyro(0,2).drive(-600, 60, 1).waitUntilSettled();
  chassis.turn(300, 127,1).withTol(10).waitUntilSettled();
  //grab 3 cubes

  chassis.drive(-200, 100);
  arm.tower(2);
  chassis.waitUntilSettled();
  chassis.drive(200, 100);
  arm.move(1.5, 127);
  chassis.waitUntilSettled();
  roller(-127);
  //score red side mid height tower

  delay(1000);
  chassis.withGyro(0,2).drive(-700, 100);
  delay(300);
  arm.zero();
  chassis.waitUntilSettled();
  chassis.withTarget(700, 70, 90, 1,4).withTarget(2200, 50, 95, 1,4).withTarget(2000, 70, 95, 1,4).withTarget(-5000, 100, 95, 1,4).drive().waitUntilSettled();
  //try to grab a full rack of cubes and drive back to the waitUntilSettled

  chassis.left(-80);
  chassis.right(-80);
  roller(0);
  delay(1100);
  chassis.left(0);
  chassis.right(0);
  chassis.calibrateGyro();
  //calibrate gyro pre score

  delay(500);
  roller(127);
  chassis.withTarget(500, 60, 45, 2).withTarget(1000, 60, 135, 1).drive();
  delay(2000);
  rack.move(RACK_UP,127,13).withTol(10).waitUntilSettled();
  roller(-120);
  rack.move(RACK_DOWN,127,13).withTol(10);
  chassis.drive(-300, 127,8).withTol(5).waitUntilSettled();
  //score second tower of 9 cubes
}
