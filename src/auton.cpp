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
  odom.calibrateGyro();

  // roller(127);
  // chassis.drive(2100,65,4).withAngle(0, 80).withTol(30).waitUntilSettled();
  // chassis.drive(-300,70,4).withAngle(0, 80).withTol(30).waitUntilSettled();
  // chassis.turn(45,70).withTol(10).waitUntilSettled();
  // chassis.drive(500,40,2).withAngle(45, 80).withTol(30).waitUntilSettled();

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
  roller(127);
  chassis.lock();
  chassis.drive(1050,50,2).withAngle(0,50).withTol(40).waitUntilSettled();
  chassis.drive(-1300,200,5).withAngle(65,127,3).withTol(40).waitUntilSettled();
  chassis.turn(0,127).withTol(10).waitUntilSettled();
  chassis.drive(1600,60,2).withAngle(0,50).withTol(40).waitUntilSettled();
  chassis.drive(-1500,127,2).withTol(40).waitUntilSettled();
  chassis.turn(270,80).withTol(10).waitUntilSettled();
  LF.move(100);
  LB.move(-100);
  RF.move(100);
  RB.move(-100);
  delay(800);
  rack.move(RACK_UP,127,7).withGain(0.1).withTol(30);
  roller(-0.3,50);
  delay(500);
  chassis.left(50);
  chassis.right(50);
  roller(-0.2,100);
  delay(800);
  chassis.left(30);
  chassis.right(30);
  rack.waitUntilSettled();
  rack.move(RACK_DOWN,127,8).withTol(20);
  roller(-70);
  chassis.lock();
  chassis.drive(-550,60,2).withAngle(270,50).withTol(40).waitUntilSettled();
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
chassis.lock();
  odom.zero();
  chassis.drive(250,60,2).withAngle(0,5).withTol(40);
  arm.move(1.05, 127);
  delay(800);
  chassis.setMode(0);
  arm.zero();
  chassis.left(-40);
  chassis.right(-40);
  delay(800);
  roller(127);
  chassis.drive(1600,70,2).withAngle(0,60).withTurnGain(5,0.3,3).withTol(40).waitUntilSettled();
  delay(500);
  arm.tower(1);
  delay(300);
  chassis.turn(25,70).withTol(10).waitUntilSettled();
  chassis.drive(300,60,2).withAngle(25,50).withTol(40).waitUntilSettled();
  roller(-80);
  delay(300);
  chassis.drive(-400,80,2).withAngle(25,50).withTol(40).waitUntilSettled();
  arm.zero();
  chassis.turn(0,90).withTol(10).waitUntilSettled();
  roller(127);
  chassis.drive(2700,55,2).withAngle(0,50).withTol(30).waitUntilSettled();
  delay(500);
  arm.tower(1);
  delay(100);
  chassis.turn(260,70).withTol(10).waitUntilSettled();
  chassis.drive(150,60,2).withAngle(260,50).withTol(40).waitUntilSettled();
  roller(-80);
  delay(300);
  chassis.drive(-200,60,2).withAngle(260,50).withTol(40).waitUntilSettled();
  roller(127);
  chassis.turn(350,70).withTol(10);
  delay(300);
  arm.zero();
  chassis.waitUntilSettled();
  chassis.unlock();
  chassis.drive(300,127,2).withAngle(350,50).withTol(60).waitUntilSettled();
  roller(0);
     LF.move(0);
     LB.move(127);
     RF.move(0);
     RB.move(127);
     delay(1200);
     LF.move(-100);
     LB.move(100);
     RF.move(-100);
     RB.move(100);
     delay(800);
     rack.move(RACK_UP,127,7).withGain(0.1).withTol(30);
     roller(-0.3,50);
     delay(500);
     chassis.left(50);
     chassis.right(50);
     roller(-0.2,100);
     delay(800);
     chassis.left(30);
     chassis.right(30);
     rack.waitUntilSettled();
     rack.move(RACK_DOWN,127,8).withTol(20);
     roller(-70);
     chassis.lock();
     chassis.drive(-550,60,2).withAngle(270,50).withTol(40).waitUntilSettled();
     chassis.strafe(1900, 75).withSlop(-520).withTol(40).waitUntilSettled();
     chassis.left(-70);
     chassis.right(-70);
     chassis.lock();
     delay(800);
     roller(127);
     chassis.drive(4050,75,2).withAngle(182,60).withTurnGain(5,0.3,3).withTol(40).waitUntilSettled();
     rack.move(2000, 127).withTol(0);
     delay(300);
     chassis.turn(270,70).withTol(10).waitUntilSettled();
     rack.move(RACK_DOWN, 127).withTol(0);
     chassis.drive(1300,100,2).withAngle(270,50).withTol(40);
     delay(100);
     arm.tower(1);
     chassis.waitUntilSettled();
     roller(-80);
     delay(300);
     chassis.drive(-100,127,2).withAngle(270,50).withTol(40).waitUntilSettled();
     LF.move(90);
     LB.move(-127);
     RF.move(90);
     RB.move(-127);
     delay(800);

     roller(127);
     arm.zero();
     delay(1500);
     chassis.left(100);
     chassis.right(100);
     delay(400);
     rack.move(RACK_UP,127,7).withGain(0.1).withTol(30);
     chassis.left(50);
     chassis.right(50);
     roller(-0.4,50);
     delay(500);

     roller(-0.2,100);
     delay(500);
     chassis.left(30);
     chassis.right(30);
     rack.waitUntilSettled();
     rack.move(RACK_DOWN,127,8).withTol(20);
     roller(-70);
     chassis.lock();
     chassis.drive(-4100,127,2).withAngle(275, 50).withTol(50).waitUntilSettled();
     roller(127);
     LF.move(100);
     LB.move(-100);
     RF.move(100);
     RB.move(-100);
     delay(400);
     chassis.strafe(-1700,75,2).withTol(60).waitUntilSettled();


     chassis.drive(1000,80,2).withAngle(270,50).withTol(50).waitUntilSettled();
          delay(800);
          arm.tower(2);
          delay(800);
          chassis.drive(400,127,2).withAngle(270, 50).withTol(20).waitUntilSettled();
          roller(-90);
          delay(300);
          chassis.drive(-520,127,2).withAngle(270, 50).withTol(50).waitUntilSettled();
          delay(300);
          arm.zero();
          roller(127);



     chassis.turn(358,100).withTol(10).waitUntilSettled();
     roller(127);
     chassis.drive(1100,80,2).withAngle(0,50).withTol(40).waitUntilSettled();
     delay(500);
     arm.tower(1);
     chassis.turn(35,70).withTol(10).waitUntilSettled();
     chassis.drive(300,60,2).withAngle(35,50).withTol(40).waitUntilSettled();
     roller(-80);
     delay(300);
     chassis.drive(-400,80,2).withAngle(35,50).withTol(40).waitUntilSettled();
     arm.zero();
     chassis.turn(0,90).withTol(10).waitUntilSettled();
     roller(127);
     chassis.drive(2000,127,2).withAngle(0,50).withTol(30).waitUntilSettled();
     arm.tower(2);
     delay(500);
     chassis.turn(315,127).withTol(10).waitUntilSettled();
     chassis.drive(700,90,2).withAngle(315,50).withTol(40).waitUntilSettled();
     roller(-90);
     delay(300);
     chassis.drive(-400,80,2).withAngle(315,50).withTol(40);
     delay(300);
     arm.zero();
     chassis.waitUntilSettled();




     roller(127);
  delay(100000);
  arm.move(1.05, 127);
  delay(800);
  arm.zero();
  odom.reset();
  delay(800);
  arm.move(1.05, 127);
  chassis.lock();
  arm.waitUntilSettled();

}

void skills2() {

}

void skills3() {

}
