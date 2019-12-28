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
  std::cout << "Testing" << std::endl;


chassis.lock();
arm.move(ARM_LOW_TOWER, 127).withTol(20).waitUntilSettled();
arm.zero();
delay(200);
  chassis.drive({1388,0}, 70,4).withTol(70);
  delay(400);
  roller(127);
  chassis.waitUntilSettled();

  chassis.drive({120,-900}, 100,4,true).withTol(150).waitUntilSettled();
  roller(127);

  chassis.drive({1300,-1000}, 50,4).withPoint({1980,-800}, 50,4).withTol(200).drive().waitUntilSettled();

  chassis.drive({607,-1017}, 90,4,true).withConst(0.1).withTol(100).waitUntilSettled();
  chassis.turn({270,-1420}, 70,4).withConst(0.1).withTol(20).waitUntilSettled();

  chassis.drive({270,-1420}, 85,4).withTol(300);
  delay(300);
  roller(-0.4,60);
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
  chassis.drive({674,-1000}, 127,4,true).withConst(0.2).withTol(70).waitUntilSettled();
//   roller(127);
//
//   chassis.drive({4295,-980}, 60,4).withConst(0.2).withTol(70).waitUntilSettled();
//   delay(800);
//   arm.tower(1);
//   delay(500);
//
//
//   chassis.turn({4200,-1277}, 127,4).withConst(0.2).withTol(70).waitUntilSettled();
//   chassis.drive({4200,-1300}, 80,4).withConst(0.2).withTol(70).waitUntilSettled();
//
//
//
//   roller(-80);
//   delay(600);
//   chassis.drive({4195,-1123}, 80,4,true).withConst(0.4).withTol(70).waitUntilSettled();
//   roller(127);
//   arm.zero();
//   chassis.drive({4650,-1246}, 80,4).withConst(0.2).withTol(70).waitUntilSettled();
//   roller(-0.4,60);
//   chassis.drive({5000,-1600}, 80,4).withConst(0.2).withTol(250);
//   delay(700);
//   rack.move(RACK_UP,127,8).withTol(30);
//   chassis.waitUntilSettled();
//   chassis.left(60);
//   chassis.right(60);
//   delay(400);
//   roller(0);
//
// delay(200);
// chassis.left(0);
// chassis.right(0);
// rack.waitUntilSettled();
// rack.move(RACK_DOWN,127,8).withTol(20);
//
// roller(-90);
// chassis.withPoint({4800,-1400}, 80,4,true).withPoint({5153,156}, 80,4,true).withConst(1).withTol(60).drive();
// delay(1000);
// roller(127);
// chassis.waitUntilSettled();
// chassis.drive({3348,172}, 80,4).withConst(0.2).withTol(60).waitUntilSettled();
// arm.tower(1).waitUntilSettled();
// chassis.drive({3282,35}, 80,4).withConst(0.2).withTol(60).waitUntilSettled();
// roller(-80);
// delay(500);
//
// chassis.drive({3348,172}, 80,4,true).withConst(0.2).withTol(60).waitUntilSettled();
// arm.tower(2);
// chassis.drive({1496,371}, 80,4).withConst(0.2).withTol(60).waitUntilSettled();
// chassis.drive({1059,759}, 80,4).withConst(0.2).withTol(60).waitUntilSettled();
// roller(-80);
//
//
// chassis.lock();






















































delay(10000);

//odom.reset();

//   delay(100000);
roller(127);
chassis.drive(1500, 80).withTol(50).waitUntilSettled();
chassis.strafe(300, 50).withTol(50).waitUntilSettled();
chassis.drive(1500, 80).withTol(50).waitUntilSettled();
chassis.strafe(-100, 50).withTol(50).waitUntilSettled();
chassis.drive(1600, 80).withTol(50).waitUntilSettled();
chassis.turn(270, 100, 4).withTol(50).waitUntilSettled();
        LF.move(-100);
        LB.move(100);
        RF.move(-100);
        RB.move(100);
delay(2000);

chassis.left(50);
chassis.right(50);
rack.move(RACK_UP,127,5).withTol(20);

roller(10);


delay(800);
chassis.left(10);
chassis.right(10);
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
chassis.turn(270,50,4).waitUntilSettled();
chassis.strafe(1000, 127).withSlop(0).withTol(20).waitUntilSettled();

LF.move(90);
LB.move(-90);
RF.move(90);
RB.move(-90);
delay(1500);

chassis.left(50);
chassis.right(50);
roller(-0.5,50);
rack.move(RACK_UP,127,5).withTol(20);



delay(400);
chassis.left(10);
chassis.right(10);
rack.waitUntilSettled();
rack.move(RACK_DOWN,127,8).withTol(20);

roller(-70);
chassis.lock();
chassis.drive(-500, 60, 10).withTol(60).waitUntilSettled();


delay(10000);


  arm.move(1.05, 127).withTol(40);
  arm.waitUntilSettled();
  arm.zero().waitUntilSettled();
  roller(127);
  // arm.move(1.05, 127);

  //
  // chassis.lock();
  // chassis.withPoint({50, 0}, 30, 5)
  //         .withPoint({700, -535}, 60, 5)
  //         .withConst(2).withTol(100).drive().waitUntilSettled();
  // //deploy, drive to tower
  //
  // arm.waitUntilSettled();
  // roller(-60);
  // delay(600);
  // //score red tower
  //
  // roller(127);
  // chassis.drive({100, 0}, 80, 6, true).withTol(60);
  // delay(200);
  // arm.zero().waitUntilSettled();
  // chassis.waitUntilSettled();
  // delay(300);
  // chassis.unlock();
  // chassis.waitUntilSettled();
  // chassis.turn({1671, -185},60,5).withTol(20).waitUntilSettled();
  chassis.withPoint({1500, -185}, 50, 12)
          .withPoint({2965, -195}, 50, 12)
          .withPoint({4000, -175}, 50, 12)
          .withPoint({4760, -538}, 50, 12)
          .withConst(2).withTol(200).drive().waitUntilSettled();

  // chassis.withPoint({1500, -185}, 50, 12)
  //         .withPoint({2965, -195}, 50, 12)
  //         .withPoint({4067, -175}, 50, 12)
  //         .withPoint({4960, -538}, 50, 12)
  //         .withConst(2).withTol(200).drive().waitUntilSettled();
          chassis.left(60);
          chassis.right(60);
  rack.move(RACK_UP,127,5).withTol(30);

  roller(10);


  delay(200);
  chassis.left(0);
  chassis.right(0);
  rack.waitUntilSettled();
  rack.move(RACK_DOWN,127,8).withTol(20);

  roller(-90);
  chassis.lock();
  chassis.drive({4500, -200}, 60, 10, true).withTol(60).waitUntilSettled();
  arm.move(1.05, 127);
  roller(127);
  chassis.withPoint({4566, 500}, 60, 6, true)
  .withPoint({4964, 877}, 60, 6, true).withTol(60).drive().waitUntilSettled();
  arm.zero().waitUntilSettled();
  chassis.drive({1000, 1200}, 70, 6).withTol(60).waitUntilSettled();
  // arm.tower(1);
  // delay(1600);
  // chassis.turn(-125,100,5).withTol(20).waitUntilSettled();
  // roller(-80);
  // delay(500);
  // delay(600);
  //
  // chassis.turn({1000, 1200}, 50, 6).withTol(10);
  // delay(200);
  roller(127);
  chassis.waitUntilSettled();
  delay(500);
  arm.tower(2);
  delay(1000);
  arm.move(ARM_MID_TOWER+0.2, 120);
  delay(1000);
  chassis.turn({900, 1400}, 60, 6).withTol(60).waitUntilSettled();
  chassis.drive({871, 1525}, 60, 6).withTol(60).waitUntilSettled();
  roller(-70);
  delay(500);
 chassis.drive({1331, 239}, 60, 6,true).withTol(60);
 delay(400);
 arm.zero();
 roller(127);
 chassis.waitUntilSettled();
 chassis.drive({172, -284}, 60, 6).withTol(150).waitUntilSettled();


 chassis.left(60);
 chassis.right(60);
rack.move(RACK_UP,127,5).withTol(30);
delay(400);
roller(0);
roller(-0.6,60);

delay(200);
chassis.left(0);
chassis.right(0);
rack.waitUntilSettled();
rack.move(RACK_DOWN,127,8).withTol(20);
roller(-90);
chassis.lock();
chassis.drive({1800, 800}, 80, 6,true).withTol(150).waitUntilSettled();
roller(127);
chassis.turn({1900, 4227}, 80, 6).withTol(20).waitUntilSettled();
chassis.drive({1900, 4227}, 80, 6).withTol(150).waitUntilSettled();
chassis.drive({1900, 3000}, 80, 6,true).withTol(150).waitUntilSettled();
arm.tower(1);
delay(200);
chassis.turn(0,50,4).withTol(10).waitUntilSettled();
chassis.drive({2173,3100},50,4).waitUntilSettled();
roller(-80);







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
       delay(100);
       roller(127);
       // chassis.withGyro(0,2).drive(2300, 50,2).waitUntilSettled();
       roller(50);
       // chassis.withGyro(0,2).drive(-1500, 80,1).waitUntilSettled();
       roller(127);
       chassis.turn(550, 80, 5).withConst(1.1).waitUntilSettled();
       delay(200);
       // chassis.withGyro(-130,2).drive(750, 100,2);
       chassis.unlock();
       roller(-0.3,70);
       rack.move(RACK_UP,127,13).withTol(20);
       delay(200);

       rack.waitUntilSettled();
       roller(-80);
       delay(50);
       rack.move(RACK_DOWN,127,13).withTol(20);
       //chassis.drive(-500, 127,5).withTol(5).waitUntilSettled();

       io::master.rumble(" .");


    // chassis.calibrateGyro();
    //  delay(500);
    //  deploy(0.7);
    //  delay(200);
    //  roller(127);
    //  delay(100);
    //  roller(127);
    //  //chassis.drive(1700, 60,7).withTol(5).waitUntilSettled();
    //  roller(0);
    //  chassis.lock();
    //  chassis.withGyro(50,1).drive(-1900, 127,7).waitUntilSettled();
    //  roller(127);
    //  roller(127);
    //  chassis.withGyro(0,2).drive(1700, 70,1).waitUntilSettled();
    //  chassis.withGyro(0,2).drive(-1050, 100,1).waitUntilSettled();
    //  roller(127);
    //  chassis.turn(-450, 80, 5).withConst(1.1).waitUntilSettled();
    //  delay(200);
    //  chassis.withGyro(-130,2).drive(550, 100,2);
    //  chassis.unlock();
    //  roller(-0.3,70);
    //  rack.move(RACK_UP,127,13).withTol(20);
    //  delay(200);
    //
    //  rack.waitUntilSettled();
    //  roller(-80);
    //  delay(50);
    //  rack.move(RACK_DOWN,127,13).withTol(20);
    //  //chassis.drive(-500, 127,5).withTol(5).waitUntilSettled();
    //
    //  io::master.rumble(" .");






  //
  //
  // chassis.calibrateGyro();
  // delay(500);
  // deploy(0.7);
  // delay(200);
  // roller(127);
  // delay(100);
  // roller(127);
  // //chassis.drive(2300, 50,7).withTol(5).waitUntilSettled();
  // roller(0);
  // chassis.withGyro(-45,1).drive(-1900, 127,10).waitUntilSettled();
  // roller(127);
  // delay(100);
  // roller(127);
  // chassis.withGyro(0,2).drive(1400, 70,1).waitUntilSettled();
  // roller(127);
  // chassis.turn(450, 80, 5).withConst(1.1).waitUntilSettled();
  //
  // chassis.withGyro(-128,0.7).drive(2350, 100,3);
  // chassis.unlock();
  // delay(500);
  // roller(-0.3,70);
  // delay(700);
  // rack.move(RACK_UP,127,13).withTol(20);
  // delay(200);
  //
  // rack.waitUntilSettled();
  // roller(-80);
  // delay(50);
  // rack.move(RACK_DOWN,127,13).withTol(20);
  // //chassis.drive(-500, 127,5).withTol(5).waitUntilSettled();
  //
  // io::master.rumble(" .");
}

void redbigzone() {
  chassis.calibrateGyro();
  delay(400);
  arm.move(0.5, 127);
  delay(600);
  deploy(0.3);
  arm.zero();
  delay(300);
  roller(127);
  arm.waitUntilSettled();
  delay(200);
  // //chassis.drive(300, 100).waitUntilSettled();
  // chassis.withTarget(700, 50, -85, 1, 1).withTarget(1600, 60, -25,2 ,3).withTarget(2600, 60, 90, 1).drive().waitUntilSettled();

}

void redbigzonescore() {
  chassis.calibrateGyro();
  delay(400);
  arm.move(0.5, 127);
  delay(600);
  deploy(0.3);
  arm.zero();
  delay(300);
  roller(127);
  arm.waitUntilSettled();
  delay(200);
  rack.move(RACK_DOWN+300, 127);
  // //chassis.drive(300, 100).waitUntilSettled();
  // chassis.withTarget(700, 50, -85, 1, 1).withTarget(1600, 60, -25,2 ,3).withTarget(2500, 60, 90, 1).drive().waitUntilSettled();
  // chassis.withTarget(-1600, 100, 25, 3, 3).withTarget(-300, 100, 90,2 ,3).withTarget(500, 100, 105, 1).drive().waitUntilSettled();
  chassis.left(40);
  chassis.right(40);
  delay(500);
  roller(-0.5,60);
  rack.move(RACK_UP,127,13).withTol(20).waitUntilSettled();
  roller(-100);
  rack.move(RACK_DOWN,127,13).withTol(10);
  //chassis.drive(-500, 127,5).withTol(5).waitUntilSettled();
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
  delay(100);
  roller(127);
  // chassis.withGyro(0,2).drive(2300, 50,2).waitUntilSettled();
  roller(50);
  // chassis.withGyro(0,2).drive(-1500, 80,1).waitUntilSettled();
  roller(127);
  chassis.turn(-550, 80, 5).withConst(1.1).waitUntilSettled();
  delay(200);
  // chassis.withGyro(130,2).drive(750, 100,2);
  chassis.unlock();
  roller(-0.3,70);
  rack.move(RACK_UP,127,13).withTol(20);
  delay(200);

  rack.waitUntilSettled();
  roller(-80);
  delay(50);
  rack.move(RACK_DOWN,127,13).withTol(20);
  //chassis.drive(-500, 127,5).withTol(5).waitUntilSettled();

  io::master.rumble(" .");

  //
  // chassis.calibrateGyro();
  //  delay(500);
  //  deploy(0.7);
  //  delay(200);
  //  roller(127);
  //  delay(100);
  //  roller(127);
  //  //chassis.drive(1700, 60,7).withTol(5).waitUntilSettled();
  //  roller(0);
  //  chassis.lock();
  //  chassis.withGyro(-50,1).drive(-1900, 127,7).waitUntilSettled();
  //  roller(127);
  //  roller(127);
  //  chassis.withGyro(0,2).drive(1700, 70,1).waitUntilSettled();
  //  chassis.withGyro(0,2).drive(-1050, 100,1).waitUntilSettled();
  //  roller(127);
  //  chassis.turn(-450, 80, 5).withConst(1.1).waitUntilSettled();
  //  delay(200);
  //  chassis.withGyro(130,2).drive(550, 100,2);
  //  chassis.unlock();
  //  roller(-0.3,70);
  //  rack.move(RACK_UP,127,13).withTol(20);
  //  delay(200);
  //
  //  rack.waitUntilSettled();
  //  roller(-80);
  //  delay(50);
  //  rack.move(RACK_DOWN,127,13).withTol(20);
  //  //chassis.drive(-500, 127,5).withTol(5).waitUntilSettled();
  //
  //  io::master.rumble(" .");
  //


  // chassis.calibrateGyro();
  //  delay(500);
  //  deploy(0.7);
  //  delay(200);
  //  roller(127);
  //  delay(100);
  //  roller(127);
  //  //chassis.drive(2300, 50,7).withTol(5).waitUntilSettled();
  //  roller(0);
  //  chassis.withGyro(45,1).drive(-1900, 127,10).waitUntilSettled();
  //  roller(127);
  //  delay(100);
  //  roller(127);
  //  chassis.withGyro(0,2).drive(1500, 70,1).waitUntilSettled();
  //  roller(127);
  //  chassis.turn(-450, 80, 5).withConst(0.7).waitUntilSettled();
  //  delay(100);
  //  chassis.withGyro(135,0.7).drive(2350, 100,3);
  //  chassis.unlock();
  //  delay(500);
  //  roller(-0.3,70);
  //  delay(700);
  //  rack.move(RACK_UP,127,13).withTol(20);
  //  delay(200);
  //
  //  rack.waitUntilSettled();
  //  roller(-80);
  //  delay(50);
  //  rack.move(RACK_DOWN,127,13).withTol(20);
  //  //chassis.drive(-500, 127,5).withTol(5).waitUntilSettled();
  //
  //  io::master.rumble(" .");
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
  // chassis.drive(300, 100).waitUntilSettled();
  // chassis.withTarget(700, 40, 85, 1, 1).withTarget(1600, 40, 25,2 ,3).withTarget(2600, 40, -90, 1).drive().waitUntilSettled();
}

void bluebigzonescore() {
  chassis.calibrateGyro();
  delay(400);
  arm.move(0.5, 127);
  delay(600);
  deploy(0.3);
  arm.zero();
  delay(300);
  roller(127);
  arm.waitUntilSettled();
  delay(200);
  rack.move(RACK_DOWN+300, 127);
  // chassis.drive(300, 100).waitUntilSettled();
  // chassis.withTarget(700, 50, 85, 1, 1).withTarget(1600, 60, 25,2 ,3).withTarget(2500, 60, -90, 1).drive().waitUntilSettled();
  // chassis.withTarget(-1600, 100, -25, 3, 3).withTarget(-300, 100, -90,2 ,3).withTarget(500, 100, -105, 1).drive().waitUntilSettled();
  chassis.left(40);
  chassis.right(40);
  delay(500);
  roller(-0.5,60);
  rack.move(RACK_UP,127,13).withTol(20).waitUntilSettled();
  roller(-100);
  rack.move(RACK_DOWN,127,13).withTol(10);
  // chassis.drive(-500, 127,5).withTol(5).waitUntilSettled();
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
  chassis.turn({1671, -185},60,5).withTol(20).waitUntilSettled();
  chassis.withPoint({1500, -185}, 50, 12)
          .withPoint({2965, -195}, 50, 12)
          .withPoint({4000, -175}, 50, 12)
          .withPoint({4760, -538}, 50, 12)
          .withConst(2).withTol(200).drive().waitUntilSettled();

  // chassis.withPoint({1500, -185}, 50, 12)
  //         .withPoint({2965, -195}, 50, 12)
  //         .withPoint({4067, -175}, 50, 12)
  //         .withPoint({4960, -538}, 50, 12)
  //         .withConst(2).withTol(200).drive().waitUntilSettled();
  chassis.left(60);
  chassis.right(60);
  rack.move(RACK_UP,127,5).withTol(30);
  delay(400);
  roller(0);
  roller(-0.1,60);

  delay(200);
  chassis.left(0);
  chassis.right(0);
  rack.waitUntilSettled();
  rack.move(RACK_DOWN,127,8).withTol(20);

  roller(-90);
  chassis.lock();
  chassis.drive({4616, -292}, 80, 10, true).withTol(60).waitUntilSettled();
  arm.move(1.05, 127);
  chassis.drive({4566, 500}, 80, 6, true).withTol(60).waitUntilSettled();
  roller(127);
  chassis.drive({4964, 877}, 80, 6, true).withTol(60).waitUntilSettled();
  arm.zero().waitUntilSettled();
  chassis.drive({3000, 1050}, 80, 6).withTol(60).waitUntilSettled();
  arm.tower(1);
  delay(1600);
  chassis.turn(-125,100,5).withTol(20).waitUntilSettled();
  roller(-80);
  delay(500);
  delay(600);

  chassis.turn({1000, 1200}, 50, 6).withTol(10);
  delay(200);
  roller(127);
  arm.zero().waitUntilSettled();
  chassis.drive({1000, 1200}, 70, 6).withTol(60);
  roller(127);
  chassis.waitUntilSettled();
  delay(500);
  arm.tower(2);
  delay(400);
  arm.move(ARM_MID_TOWER+0.2, 120);
  delay(1000);
  chassis.turn({900, 1400}, 80, 6).withTol(60).waitUntilSettled();
  chassis.drive({871, 1525}, 80, 6).withTol(60).waitUntilSettled();
  roller(-70);
  delay(500);
  chassis.drive({1331, 239}, 80, 6,true).withTol(60);
  delay(400);
  arm.zero();
  roller(127);
  chassis.waitUntilSettled();
  chassis.drive({172, -284}, 80, 6).withTol(150).waitUntilSettled();


  chassis.left(60);
  chassis.right(60);
  rack.move(RACK_UP,127,5).withTol(30);
  delay(400);
  roller(0);
  roller(-0.6,60);

  delay(200);
  chassis.left(0);
  chassis.right(0);
  rack.waitUntilSettled();
  rack.move(RACK_DOWN,127,8).withTol(20);
  roller(-90);
  chassis.lock();
  chassis.drive({1800, 800}, 80, 6,true).withTol(150).waitUntilSettled();
  roller(127);
  chassis.turn({1900, 4227}, 80, 6).withTol(20).waitUntilSettled();
  chassis.drive({1900, 4227}, 80, 6).withTol(150).waitUntilSettled();
  chassis.drive({1900, 3000}, 80, 6,true).withTol(150).waitUntilSettled();
  arm.tower(1);
  delay(200);
  chassis.turn(0,50,4).withTol(10).waitUntilSettled();
  chassis.drive({2173,3100},50,4).waitUntilSettled();
  roller(-80);
}
