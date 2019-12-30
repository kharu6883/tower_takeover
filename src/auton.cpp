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
static Odometry odom;

using namespace io;

/*===========================================
  DICTIONARY
===========================================*/
// IDLE = 0
// DRIVING_POINT = 1
// DRIVING_DIST = 2
// TURNING = 3
// STRAFING = 4

// Ignore below. Just for testing stuff.
void tester() {


  arm.zero();

}

/*===========================================
  PREMADE FUNCTIONS
===========================================*/

void deploy() {
  arm.move(ARM_LOW_TOWER, 127).withTol(0.3).waitUntilSettled();
  arm.zero().waitUntilSettled();
}

/*===========================================
  RED MATCH AUTONOMOUSES
===========================================*/
void redsmallzone9() {
  chassis.lock();
  deploy();
  delay(200);
    chassis.drive({1420,0}, 60,4).withTol(70);
    delay(400);
    roller(127);
    chassis.waitUntilSettled();

    chassis.drive({120,-900}, 100,4,true).withTol(150).waitUntilSettled();
    roller(127);

    chassis.drive({1300,-1000}, 50,4).withPoint({1980,-800}, 60,4).withTol(200).drive().waitUntilSettled();

    chassis.drive({607,-1017}, 100,4,true).withConst(0.1).withTol(100).waitUntilSettled();
    chassis.turn({270,-1420}, 85,4).withTol(50).waitUntilSettled();

    chassis.drive({270,-1420}, 85,4).withTol(300);
    roller(-0.4,20);
    rack.move(RACK_UP,127,8).withTol(30);
    chassis.waitUntilSettled();

    chassis.left(60);
    chassis.right(60);
  delay(400);
  roller(0);

  delay(200);
  chassis.left(0);
  chassis.right(0);
  rack.waitUntilSettled();
  rack.move(RACK_DOWN,127,8).withTol(20);

  roller(-90);
  chassis.lock();
    chassis.drive(-500, 127,4).withTol(70).waitUntilSettled();

}

void redsmallzone8() {
  chassis.lock();
  arm.move(ARM_LOW_TOWER, 127).withTol(0.3).waitUntilSettled();
  arm.zero();
  delay(100);
    chassis.drive({1420,0}, 60,4).withTol(70);
    delay(300);
    roller(127);
    chassis.waitUntilSettled();

    chassis.drive({120,-1000}, 127,4,true).withTol(150).waitUntilSettled();
    roller(127);
chassis.turn({1500,-1000}, 80,4).withTol(20).waitUntilSettled();
    chassis.drive({1500,-1000}, 60,4).withTol(100).waitUntilSettled();

    chassis.drive({300,-900}, 127,4,true).withConst(0.1).withTol(100).waitUntilSettled();
    chassis.turn(270,70).withTol(20).waitUntilSettled();
    LF.move(100);
    LB.move(-100);
    RF.move(100);
    RB.move(-100);
    delay(1200);

    chassis.left(50);
    chassis.right(50);
    rack.move(RACK_UP,127,5).withConst(0.11 ).withTol(20);
    roller(-0.4,50);


    delay(800);
    chassis.left(30);
    chassis.right(30);
    rack.waitUntilSettled();
    rack.move(RACK_DOWN,127,8).withTol(20);

    roller(-70);
    chassis.lock();
    chassis.drive(-500, 60, 10).withTol(60).waitUntilSettled();


}

void redsmallzone5() {
  chassis.lock();
  arm.move(ARM_LOW_TOWER, 127).withTol(0.3).waitUntilSettled();
  arm.zero().waitUntilSettled();
  delay(200);
    chassis.drive({1420,0}, 60,4).withTol(70);
    delay(400);
    roller(127);
    chassis.waitUntilSettled();

    chassis.drive({300,100}, 100,4,true).withConst(0.1).withTol(100).waitUntilSettled();
    chassis.turn(270, 85,4).withTol(30).waitUntilSettled();
    LF.move(100);
    LB.move(-100);
    RF.move(100);
    RB.move(-100);
    delay(1200);

    chassis.left(50);
    chassis.right(50);
    rack.move(RACK_UP,127,5).withTol(20);
    roller(-0.4,50);


    delay(800);
    chassis.left(30);
    chassis.right(30);
    rack.waitUntilSettled();
    rack.move(RACK_DOWN,127,8).withTol(20);

    roller(-70);
    chassis.lock();
    chassis.drive(-500, 60, 10).withTol(60).waitUntilSettled();

}

void redbigzone() {
  chassis.lock();
  arm.move(ARM_LOW_TOWER, 127).waitUntilSettled();
  arm.zero().waitUntilSettled();
    roller(127);
    chassis.withPoint({206,0}, 60,4).
    withPoint({450,-327}, 60,4).
    withPoint({1400,-900}, 60,4).
    withPoint({1600,900}, 60,4).withTol(60).drive().waitUntilSettled();

}

void redbigzonescore() {
  chassis.lock();
  arm.move(ARM_LOW_TOWER, 127).waitUntilSettled();
  arm.zero().waitUntilSettled();
    roller(127);
    chassis.withPoint({206,0}, 100,4).
    withPoint({450,-327}, 70,4).
    withPoint({1400,-900}, 60,4).
    withPoint({1600,900}, 80,4).withTol(60).drive().waitUntilSettled();
    chassis.drive({120,300}, 127,4,true).withTol(150).waitUntilSettled();
    chassis.turn(90, 127,4).withTol(15).waitUntilSettled();
    LF.move(-127);
    LB.move(127);
    RF.move(-127);
    RB.move(127);
    delay(700);
    chassis.drive(400,127,8).withTol(70).withConst(2);
    delay(100);
    rack.move(RACK_UP,127,8).withTol(20);
    delay(300);
    chassis.waitUntilSettled();
    chassis.left(40);
    chassis.right(40);
    roller(-0.6,50);



    delay(1000);
    chassis.left(10);
    chassis.right(10);
    rack.waitUntilSettled();
    rack.move(RACK_DOWN,127,8).withTol(20);

    roller(-70);
    chassis.lock();
    chassis.drive(-500, 60, 10).withTol(60).waitUntilSettled();



}

/*===========================================
  BLUE MATCH AUTONOMOUSES
===========================================*/
void bluesmallzone9() {
  chassis.lock();
  deploy();
  delay(200);
    chassis.drive({1420,0}, 60,4).withTol(70);
    delay(400);
    roller(127);
    chassis.waitUntilSettled();

    chassis.drive({120,900}, 100,4,true).withTol(150).waitUntilSettled();
    roller(127);

    chassis.drive({1300,1000}, 50,4).withPoint({1980,800}, 60,4).withTol(200).drive().waitUntilSettled();

    chassis.drive({607,1017}, 100,4,true).withConst(0.1).withTol(100).waitUntilSettled();
    chassis.turn({270,1420}, 85,4).withTol(50).waitUntilSettled();

    chassis.drive({270,1420}, 85,4).withTol(300);
    roller(-0.4,20);
    rack.move(RACK_UP,127,8).withTol(30);
    chassis.waitUntilSettled();

    chassis.left(60);
    chassis.right(60);
  delay(400);
  roller(0);

  delay(200);
  chassis.left(0);
  chassis.right(0);
  rack.waitUntilSettled();
  rack.move(RACK_DOWN,127,8).withTol(20);

  roller(-90);
  chassis.lock();
    chassis.drive(-500, 127,4).withTol(70).waitUntilSettled();


}

void bluesmallzone8() {
  chassis.lock();
  arm.move(ARM_LOW_TOWER, 127).withTol(0.3).waitUntilSettled();
  arm.zero();
  delay(100);
    chassis.drive({1420,0}, 60,4).withTol(70);
    delay(300);
    roller(127);
    chassis.waitUntilSettled();

    chassis.drive({120,1000}, 127,4,true).withTol(150).waitUntilSettled();
    roller(127);
  chassis.turn({1500,1000}, 80,4).withTol(20).waitUntilSettled();
    chassis.drive({1500,1000}, 60,4).withTol(100).waitUntilSettled();

    chassis.drive({300,900}, 127,4,true).withConst(0.1).withTol(100).waitUntilSettled();
    chassis.turn(90,70).withTol(20).waitUntilSettled();
    LF.move(-100);
    LB.move(100);
    RF.move(-100);
    RB.move(100);
    delay(1200);

    chassis.left(50);
    chassis.right(50);
    rack.move(RACK_UP,127,5).withConst(0.11 ).withTol(20);
    roller(-0.4,50);


    delay(800);
    chassis.left(30);
    chassis.right(30);
    rack.waitUntilSettled();
    rack.move(RACK_DOWN,127,8).withTol(20);

    roller(-70);
    chassis.lock();
    chassis.drive(-500, 60, 10).withTol(60).waitUntilSettled();

}

void bluesmallzone5() {
  chassis.lock();
  arm.move(ARM_LOW_TOWER, 127).withTol(0.3).waitUntilSettled();
  arm.zero().waitUntilSettled();
  delay(200);
    chassis.drive({1420,0}, 60,4).withTol(70);
    delay(400);
    roller(127);
    chassis.waitUntilSettled();

    chassis.drive({300,-100}, 100,4,true).withConst(0.1).withTol(100).waitUntilSettled();
    chassis.turn(90, 85,4).withTol(30).waitUntilSettled();
    LF.move(-100);
    LB.move(100);
    RF.move(-100);
    RB.move(100);
    delay(1200);

    chassis.left(50);
    chassis.right(50);
    rack.move(RACK_UP,127,5).withTol(20);
    roller(-0.4,50);


    delay(800);
    chassis.left(30);
    chassis.right(30);
    rack.waitUntilSettled();
    rack.move(RACK_DOWN,127,8).withTol(20);

    roller(-70);
    chassis.lock();
    chassis.drive(-500, 60, 10).withTol(60).waitUntilSettled();

}

void bluebigzone() {
  chassis.lock();
  arm.move(ARM_LOW_TOWER, 127).waitUntilSettled();
  arm.zero().waitUntilSettled();
    roller(127);
    chassis.withPoint({206,0}, 60,4).
    withPoint({450,327}, 60,4).
    withPoint({1400,900}, 60,4).
    withPoint({1600,-900}, 60,4).withTol(60).drive().waitUntilSettled();

}

void bluebigzonescore() {
  chassis.lock();
  arm.move(ARM_LOW_TOWER, 127).waitUntilSettled();
  arm.zero().waitUntilSettled();
    roller(127);
    chassis.withPoint({206,0}, 100,4).
    withPoint({450,327}, 70,4).
    withPoint({1400,900}, 60,4).
    withPoint({1600,-900}, 80,4).withTol(60).drive().waitUntilSettled();
    chassis.drive({120,-300}, 127,4,true).withTol(150).waitUntilSettled();
    chassis.turn(270, 127,4).withTol(15).waitUntilSettled();
    LF.move(127);
    LB.move(-127);
    RF.move(127);
    RB.move(-127);
    delay(700);
    chassis.drive(400,127,8).withTol(70).withConst(2);
    delay(100);
    rack.move(RACK_UP,127,8).withTol(20);
    delay(300);
    chassis.waitUntilSettled();
    chassis.left(40);
    chassis.right(40);
    roller(-0.6,50);



    delay(1000);
    chassis.left(10);
    chassis.right(10);
    rack.waitUntilSettled();
    rack.move(RACK_DOWN,127,8).withTol(20);

    roller(-70);
    chassis.lock();
    chassis.drive(-500, 60, 10).withTol(60).waitUntilSettled();


}

/*===========================================
  SKILLS AUTONOMOUSES
===========================================*/
void skills1() {

  arm.move(1.05, 127);
  chassis.lock();
  chassis.withPoint({50, 0}, 30, 5)
          .withPoint({700, -535}, 60, 5)
          .withConst(2).withTol(100).drive().waitUntilSettled();
  //deploy, drive to tower

  arm.waitUntilSettled();
  roller(-60);
  delay(600);
  //score red tower

  roller(127);
  chassis.drive({100, 0}, 80, 6, true).withTol(60);
  delay(200);
  arm.zero().waitUntilSettled();
  chassis.waitUntilSettled();
  delay(300);
  chassis.unlock();
  chassis.waitUntilSettled();
  chassis.turn({1671, -185},70,5).withTol(20).waitUntilSettled();
  chassis.withPoint({1500, -185}, 70, 12)
          .withPoint({2965, -195}, 70, 12)
          .withPoint({4900, -175}, 70, 12)
          .withConst(2).withTol(200).drive().waitUntilSettled();


          //score nine cube down

roller(0);
          LF.move(0);
          LB.move(127);
          RF.move(0);
          RB.move(127);
          delay(1100);
          LF.move(-100);
          LB.move(100);
          RF.move(-100);
          RB.move(100);
          delay(1200);

          chassis.left(50);
          chassis.right(50);
          rack.move(RACK_UP,127,5).withTol(20);
          roller(-0.4,50);


          delay(800);
          chassis.left(30);
          chassis.right(30);
          rack.waitUntilSettled();
          rack.move(RACK_DOWN,127,8).withTol(20);

          roller(-70);
          chassis.lock();
          chassis.drive(-500, 60, 10).withTol(60).waitUntilSettled();
          chassis.strafe(1800, 50).withSlop(-580).withTol(20).waitUntilSettled();

          chassis.left(-70);
          chassis.right(-70);
          delay(1100);
          roller(127);
          chassis.drive(4500, 70, 10).withTol(60).waitUntilSettled();
          chassis.strafe(-2000, 127).withSlop(0).withTol(20).waitUntilSettled();
          chassis.turn(270,50,4).withTol(8).waitUntilSettled();
          chassis.strafe(1000, 127).withSlop(0).withTol(20).waitUntilSettled();

          LF.move(127);
          LB.move(-127);
          RF.move(127);
          RB.move(-127);
          delay(1000);

          chassis.left(50);
          chassis.right(50);
          roller(-0.5,50);
          rack.move(RACK_UP,127,5).withTol(20);



          delay(400);
          chassis.left(30);
          chassis.right(30);
          rack.waitUntilSettled();
          rack.move(RACK_DOWN,127,8).withTol(20);

          roller(-70);
          chassis.lock();
          chassis.drive(-4150,127,2).withConst(0.2).withTol(50).waitUntilSettled();
          roller(127);
          LF.move(100);
          LB.move(-100);
          RF.move(100);
          RB.move(-100);
          delay(400);
          odom.reset(0);
          chassis.strafe(-1500,60,2).withTol(20).waitUntilSettled();
          roller(127);
          chassis.drive(1050,70,2).withConst(0.25).withTol(50).waitUntilSettled();
          delay(1000);
          arm.tower(2);
          delay(800);
          arm.move(1.5, 110);
          delay(500);
          chassis.drive(350,30,2).withConst(0.25).withTol(50).waitUntilSettled();
          roller(-80);
          delay(200);
          chassis.drive(-450,70,2).withConst(0.25).withTol(50).waitUntilSettled();
          delay(300);
          arm.zero();
          roller(127);
          chassis.turn(90,60,4).withConst(0.2).withTol(8).waitUntilSettled();
          chassis.left(-70);
          chassis.right(-70);
          delay(800);
          odom.reset(0);
          roller(127);
          chassis.drive(1900,70,4).withConst(0.2).withTol(50).waitUntilSettled();
          chassis.drive(-200,70,4).withConst(0.2).withTol(50).waitUntilSettled();

          arm.tower(1).waitUntilSettled();
          chassis.turn(25,60,4).withConst(0.2).withTol(8).waitUntilSettled();
          chassis.drive(100,70,4).withConst(0.8).withTol(50).waitUntilSettled();
          roller(-80);



            LF.move(0);
            LB.move(0);
            RF.move(0);
            RB.move(0);




}
