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
void redsmallzone9() {

}

void redsmallzone8() {

}

void redsmallzone7() {

    chassis.tareGyro();
    chassis.lock();
    chassis.drive(250,70,4).withTol(5);

    arm.move(1.05, 127);
    delay(800);
    arm.zero();
    odom.reset(0);
    delay(800);
      roller(127);
      chassis.waitUntilSettled();
    chassis.drive(600,55,5).withGyro(0).withTol(10).waitUntilSettled();
    chassis.drive(-1200,127,5,2).withGyro(290).withTol(10).waitUntilSettled();
    chassis.turn(0,127,4).withoutOdom().withTol(4).waitUntilSettled();
    chassis.drive(1300,60,5).withGyro(0).withTol(10).waitUntilSettled();
    rack.move(1100,127).withTol(0);
    chassis.drive(-1100,127,5).withGyro(5).withTol(10).waitUntilSettled();
    chassis.turn(90,127,4).withoutOdom().withTol(4).waitUntilSettled();
    LF.move(127);
    LB.move(-127);
    RF.move(127);
    RB.move(-127);
    delay(600);
    rack.move(RACK_UP,127,7).withConst(0.1).withTol(30);
    roller(-0.4,50);
    delay(200);
    chassis.left(50);
    chassis.right(50);
    delay(800);
    roller(-0.4,100);

    chassis.left(30);
    chassis.right(30);
    rack.waitUntilSettled();
    rack.move(RACK_DOWN,127,8).withTol(20);
    roller(-70);
    chassis.lock();
    chassis.drive(-500, 90, 10).withTol(60).waitUntilSettled();
}

void redsmallzone5() {
  chassis.tareGyro();
chassis.lock();
  chassis.drive(50,50,4).withTol(5);

  arm.move(1.05, 127);
  delay(800);
  arm.zero();
  odom.reset(0);
  delay(800);
    roller(127);
    chassis.waitUntilSettled();
  chassis.drive(950,40,5).withGyro(0).withTol(10).waitUntilSettled();

  chassis.drive(-900,60,5).withGyro(0).withTol(10).waitUntilSettled();
  rack.move(1000,127);
  chassis.turn(90,127,4).withoutOdom().withTol(4).waitUntilSettled();
  LF.move(127);
  LB.move(-127);
  RF.move(127);
  RB.move(-127);
  delay(1200);
  rack.move(RACK_UP,127,7).withConst(0.1).withTol(30);
  roller(-0.4,50);
  delay(200);
  chassis.left(50);
  chassis.right(50);
  delay(800);
  roller(-0.2,100);

  chassis.left(30);
  chassis.right(30);
  rack.waitUntilSettled();
  rack.move(RACK_DOWN,127,8).withTol(20);
  roller(-70);
  chassis.lock();
  chassis.drive(-500, 90, 10).withTol(60).waitUntilSettled();
}

void redbigzone() {
  chassis.tareGyro();
  chassis.left(-30);
  chassis.right(-30);
  arm.move(1.05, 127);
  delay(800);
  arm.zero();
  odom.reset(0);
  delay(800);
    roller(127);
    chassis.drive(600, 60,1).withGyro(0).withTol(15).waitUntilSettled();
    chassis.turn(90, 70).withoutOdom().withTol(15).waitUntilSettled();
    chassis.drive(300, 60,1,5).withGyro(90).withTol(15).waitUntilSettled();
    chassis.drive(-200, 80,1,5).withGyro(90).withTol(15).waitUntilSettled();
    chassis.turn(270, 127).withoutOdom().withTol(15).waitUntilSettled();
    chassis.drive(800, 50,1,5).withGyro(270).withTol(15).waitUntilSettled();

}

void redbigzonescore() {
  chassis.tareGyro();
  chassis.left(-30);
  chassis.right(-30);
  arm.move(1.05, 127);
  delay(800);
  arm.zero();
  odom.reset(0);
  delay(800);
    roller(127);
    chassis.drive(600, 60,1).withGyro(0).withTol(15).waitUntilSettled();
    chassis.turn(90, 70).withoutOdom().withTol(15).waitUntilSettled();
    chassis.drive(300, 60,1,5).withGyro(90).withTol(15).waitUntilSettled();
    chassis.drive(-200, 80,1,5).withGyro(90).withTol(15).waitUntilSettled();
    chassis.turn(270, 127).withoutOdom().withTol(15).waitUntilSettled();
    chassis.drive(800, 50,1,5).withGyro(270).withTol(15).waitUntilSettled();
    chassis.turn(225, 127).withoutOdom().withTol(15).waitUntilSettled();
    delay(200);
    rack.move(RACK_UP,127,7).withConst(0.1).withTol(30);
    delay(200);

    chassis.left(50);
    chassis.right(50);

    roller(-0.5,40);
    delay(600);
    rack.waitUntilSettled();
    rack.move(RACK_DOWN,127,8).withTol(30);
    chassis.setMode(IDLE);
    roller(-70);
    chassis.lock();
    chassis.drive(-350,127,2).withConst(0.2).withTol(50).waitUntilSettled();
}

/*===========================================
  BLUE MATCH AUTONOMOUSES
===========================================*/
void bluesmallzone9() {

}

void bluesmallzone8() {

}

void bluesmallzone7() {

      chassis.tareGyro();
      chassis.lock();
      chassis.drive(250,70,4).withTol(5);

      arm.move(1.05, 127);
      delay(800);
      arm.zero();
      odom.reset(0);
      delay(800);
        roller(127);
        chassis.waitUntilSettled();
      chassis.drive(600,55,5).withGyro(0).withTol(10).waitUntilSettled();
      chassis.drive(-1200,127,5,2).withGyro(70).withTol(10).waitUntilSettled();
      chassis.turn(0,127,4).withoutOdom().withTol(4).waitUntilSettled();
      chassis.drive(1300,60,5).withGyro(0).withTol(10).waitUntilSettled();
      rack.move(1100,127).withTol(0);
      chassis.drive(-1100,127,5).withGyro(355).withTol(10).waitUntilSettled();
      chassis.turn(270,127,4).withoutOdom().withTol(4).waitUntilSettled();
      LF.move(-127);
      LB.move(127);
      RF.move(-127);
      RB.move(127);
      delay(600);
      rack.move(RACK_UP,127,7).withConst(0.1).withTol(30);
      roller(-0.4,50);
      delay(200);
      chassis.left(50);
      chassis.right(50);
      delay(800);
      roller(-0.4,100);

      chassis.left(30);
      chassis.right(30);
      rack.waitUntilSettled();
      rack.move(RACK_DOWN,127,8).withTol(20);
      roller(-70);
      chassis.lock();
      chassis.drive(-500, 90, 10).withTol(60).waitUntilSettled();
}

void bluesmallzone5() {
  chassis.tareGyro();
chassis.lock();
  chassis.drive(50,50,4).withTol(5);

  arm.move(1.05, 127);
  delay(800);
  arm.zero();
  odom.reset(0);
  delay(800);
    roller(127);
    chassis.waitUntilSettled();
  chassis.drive(950,40,5).withGyro(0).withTol(10).waitUntilSettled();

  chassis.drive(-900,60,5).withGyro(0).withTol(10).waitUntilSettled();
  rack.move(1000,127);
  chassis.turn(270,127,4).withoutOdom().withTol(4).waitUntilSettled();
  LF.move(-127);
  LB.move(127);
  RF.move(-127);
  RB.move(127);
  delay(1200);
  rack.move(RACK_UP,127,7).withConst(0.1).withTol(30);
  roller(-0.4,50);
  delay(200);
  chassis.left(50);
  chassis.right(50);
  delay(800);
  roller(-0.2,100);

  chassis.left(30);
  chassis.right(30);
  rack.waitUntilSettled();
  rack.move(RACK_DOWN,127,8).withTol(20);
  roller(-70);
  chassis.lock();
  chassis.drive(-500, 90, 10).withTol(60).waitUntilSettled();
}

void bluebigzone() {

        chassis.tareGyro();
        chassis.left(-30);
        chassis.right(-30);
        arm.move(1.05, 127);
        delay(800);
        arm.zero();
        odom.reset(0);
        delay(800);
      roller(127);
      chassis.drive(600, 60,1).withGyro(0).withTol(15).waitUntilSettled();
      chassis.turn(270, 70).withoutOdom().withTol(15).waitUntilSettled();
      chassis.drive(300, 60,1,5).withGyro(270).withTol(15).waitUntilSettled();
      chassis.drive(-200, 80,1,5).withGyro(270).withTol(15).waitUntilSettled();
      chassis.turn(90, 127).withoutOdom().withTol(15).waitUntilSettled();
      chassis.drive(800, 50,1,5).withGyro(90).withTol(15).waitUntilSettled();



}

void bluebigzonescore() {

      chassis.tareGyro();
      chassis.left(-30);
      chassis.right(-30);
      arm.move(1.05, 127);
      delay(800);
      arm.zero();
      odom.reset(0);
      delay(800);
    roller(127);
    chassis.drive(600, 60,1).withGyro(0).withTol(15).waitUntilSettled();
    chassis.turn(270, 70).withoutOdom().withTol(15).waitUntilSettled();
    chassis.drive(300, 60,1,5).withGyro(270).withTol(15).waitUntilSettled();
    chassis.drive(-200, 80,1,5).withGyro(270).withTol(15).waitUntilSettled();
    chassis.turn(90, 127).withoutOdom().withTol(15).waitUntilSettled();
    chassis.drive(800, 50,1,5).withGyro(90).withTol(15).waitUntilSettled();
    chassis.turn(135, 127).withoutOdom().withTol(15).waitUntilSettled();
    delay(200);
    rack.move(RACK_UP,127,7).withConst(0.1).withTol(30);
    delay(200);

    chassis.left(50);
    chassis.right(50);

    roller(-0.5,40);
    delay(600);
    rack.waitUntilSettled();
    rack.move(RACK_DOWN,127,8).withTol(30);
    chassis.setMode(IDLE);
    roller(-70);
    chassis.lock();
    chassis.drive(-350,127,2).withConst(0.2).withTol(50).waitUntilSettled();
}

/*===========================================
  SKILLS AUTONOMOUSES
===========================================*/
void skills1() {

    chassis.tareGyro();
    chassis.left(-30);
    chassis.right(-30);
    arm.move(1.05, 127);
    delay(800);
    arm.zero();
    odom.reset(0);
    delay(800);
    arm.move(1.05, 127);
    chassis.lock();
    chassis.drive(850,80,4).withGyro(40).withConst(0.2).withTol(50).waitUntilSettled();

    arm.waitUntilSettled();
     roller(-80);
     delay(600);
     roller(127);
     chassis.drive(-650,80,4).withGyro(45).withConst(0.2).withTol(50).waitUntilSettled();
     delay(200);
     arm.zero().waitUntilSettled();
     chassis.waitUntilSettled();
     delay(100);
     chassis.lock();
     chassis.waitUntilSettled();
     chassis.turn(0,100,5).withoutOdom().withTol(20).waitUntilSettled();
     chassis.setMode(IDLE);
     chassis.drive(1200,70,5).withGyro(0).withTol(10).waitUntilSettled();
     chassis.drive(500,60,5).withGyro(10).withTol(10).waitUntilSettled();
     chassis.drive(2300,50,5).withGyro(359).withTol(10).waitUntilSettled();

              delay(500);
              arm.tower(1);
              delay(200);
     chassis.turn(90, 75,4).withoutOdom().withTol(4).waitUntilSettled();
     chassis.drive(120, 45).withGyro(90).withConst(0.6, 0.2).withTol(4).waitUntilSettled();
     roller(-80);
     delay(500);
     roller(80);
     chassis.drive(-200, 45).withGyro(90).withConst(0.6, 0.2).withTol(4).waitUntilSettled();
     arm.zero();
     chassis.unlock();
     chassis.turn(15, 75,4).withoutOdom().withTol(4).waitUntilSettled();
     delay(20);
     chassis.unlock();
     delay(20);
     chassis.drive(200, 127).withGyro(15).withConst(0.8).withTol(60).waitUntilSettled();
     roller(0);
     LF.move(0);
     LB.move(127);
     RF.move(0);
     RB.move(127);
     delay(1000);
     LF.move(-100);
     LB.move(100);
     RF.move(-100);
     RB.move(100);
     delay(500);
     rack.move(RACK_UP,127,7).withConst(0.1).withTol(30);
     roller(-0.4,50);
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
     chassis.drive(-500, 90, 10).withTol(60).waitUntilSettled();
     chassis.strafe(1780, 75).withSlop(-580).withTol(20).waitUntilSettled();
     chassis.left(-70);
     chassis.right(-70);
     chassis.lock();
     delay(400);
     roller(127);
     chassis.drive(1900, 80, 10).withGyro(180).withTol(12).waitUntilSettled();
     delay(100);
     arm.tower(1);
     delay(400);
     chassis.turn(135, 50,4).withTol(4).withoutOdom().waitUntilSettled();
     chassis.drive(200, 50).withGyro(135).withConst(0.6, 0.2).withTol(4).waitUntilSettled();
     roller(-80);
     delay(500);
     roller(127);
     chassis.lock();
     chassis.drive(-150, 50).withGyro(135).withConst(0.4, 0.2).withTol(5).waitUntilSettled();
     arm.zero();
     delay(200);
     chassis.turn(180, 55,4).withoutOdom().withTol(4).waitUntilSettled();
     chassis.drive(2400, 80, 10).withGyro(180).withTol(6).waitUntilSettled();
     chassis.strafe(-2500, 127).withSlop(-200).withTol(20).waitUntilSettled();
     chassis.turn(90,100,4).withTol(8).withoutOdom().waitUntilSettled();
     chassis.strafe(800, 127).withSlop(0).withTol(20).waitUntilSettled();
     LF.move(127);
     LB.move(-127);
     RF.move(127);
     RB.move(-127);
     delay(1000);
     rack.move(RACK_UP,127,7).withConst(0.1).withTol(30);
     roller(-0.4,50);
      chassis.smartstrafe(100, 90,40,0,5);
     delay(1000);
     roller(-0.2,100);

     chassis.setMode(IDLE);
     chassis.left(30);
     chassis.right(30);
     rack.waitUntilSettled();
     rack.move(RACK_DOWN,127,8).withTol(20);

     roller(-70);
     chassis.lock();
     chassis.drive(-4040,127,3).withConst(0.15).withTol(50).waitUntilSettled();
     roller(127);
     LF.move(100);
     LB.move(-100);
     RF.move(100);
     RB.move(-100);
     delay(400);
     odom.reset(0);
     chassis.strafe(-1400,65,2).withTol(20).waitUntilSettled();
     roller(127);
     chassis.drive(1000,80,2).withConst(0.25).withGyro(90).withTol(50).waitUntilSettled();
     delay(800);
     arm.tower(2);
     delay(700);
     arm.move(1.5, 110);
     delay(400);
     chassis.drive(480,50,2).withConst(0.25).withTol(50).waitUntilSettled();
     roller(-90);
     delay(300);
     chassis.drive(-550,80,2).withConst(0.25).withTol(50).waitUntilSettled();
     delay(300);
     arm.zero();
     roller(127);
     chassis.turn(5,127,4).withoutOdom().withTol(4).waitUntilSettled();
     odom.reset(0);
     roller(127);
     chassis.drive(1150,80,4).withGyro(5).withConst(0.2).withTol(50).waitUntilSettled();
     delay(200);
     arm.tower(1).waitUntilSettled();
     chassis.turn(335,67,4).withoutOdom().withTol(4).waitUntilSettled();
     chassis.drive(200,85,4).withConst(0.8).withTol(50).waitUntilSettled();
     roller(-80);
     delay(400);
     chassis.drive(-300,127,10).withConst(0.8).withTol(50).waitUntilSettled();
     LF.move(0);
     LB.move(0);
     RF.move(0);
     RB.move(0);


}

void skills2() {

}



void skills3() {

}
