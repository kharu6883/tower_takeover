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
// DRIVING = 1
// TURNING = 2
// ALIGNING = 3

// Ignore below. Just for testing stuff.
void tester() {
  std::cout << "Testing" << std::endl;
  roller(127);
  chassis.drive({1671, -3}, 80,12).withConst(1.3).withTol(80).waitUntilSettled();
  chassis.drive({2500, -135}, 60,12).withConst(1.3).withTol(80).waitUntilSettled();
  chassis.drive({2860, -135}, 60,12).withConst(1.3).withTol(80).waitUntilSettled();
  chassis.drive({4400, -29}, 60,12).withConst(1.3).withTol(80).waitUntilSettled();
  chassis.drive({4928, -444}, 60,12).withConst(2).withTol(100).waitUntilSettled();
  roller(60);
  rack.move(RACK_UP,127,5).withTol(20);
  chassis.waitUntilSettled();
  roller(30);
  chassis.left(60);
  chassis.right(60);
  delay(500);
  chassis.left(0);
  chassis.right(0);
  rack.waitUntilSettled();
  rack.move(RACK_DOWN,127,8).withTol(20);
  roller(-50);
  delay(90);
  roller(-90);
  chassis.left(-90);
  chassis.right(-90);
  delay(600);
  chassis.left(0);
  chassis.right(0);
  // roller(127);
  // chassis.turn({4600, 900},70).withTol(20).waitUntilSettled();
  // chassis.drive({4600, 900}, 60,12).withConst(1.3).withTol(100).waitUntilSettled();
  // delay(1000);
  // roller(-0.5,100);
  // arm.move(ARM_MID_TOWER,127);
  // delay(1200);
  // chassis.drive({4557, 1094}, 50,6).withConst(2).withTol(100).waitUntilSettled();
  // roller(-100);

  chassis.drive();

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
  // //chassis.drive(300, 100).waitUntilSettled();
//  chassis.withTarget(700, 40, 85, 1, 1).withTarget(1600, 40, 25,2 ,3).withTarget(2600, 40, -90, 1).drive().waitUntilSettled();


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
  // //chassis.drive(300, 100).waitUntilSettled();
//  chassis.withTarget(700, 50, 85, 1, 1).withTarget(1600, 60, 25,2 ,3).withTarget(2500, 60, -90, 1).drive().waitUntilSettled();
  //chassis.withTarget(-1600, 100, -25, 3, 3).withTarget(-300, 100, -90,2 ,3).withTarget(500, 100, -105, 1).drive().waitUntilSettled();
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
  SKILLS AUTONOMOUSES
===========================================*/
void skills1() {
  chassis.calibrateGyro();
  delay(500);
  arm.move(1.05, 127);
  chassis.lock();
  // chassis.withGyro(-50, 1).drive(1250, 70, 3).waitUntilSettled();
  //deploy, drive to tower

  arm.waitUntilSettled();
  roller(-60);
  delay(600);
  //score red tower

  roller(127);
  // chassis.withGyro(-15, 0.5).drive(-1000, 100, 9);
  delay(200);
  arm.zero().waitUntilSettled();
  chassis.waitUntilSettled();
  delay(300);
  chassis.unlock();
  chassis.waitUntilSettled();

  // chassis.withTarget(1500, 60, 0, 1.3,1).withTarget(400, 50, -35, 0.5).withTarget(600, 127, 0, 1.3).withTarget(600, 80, 35, 0.5).withTarget(2600, 50, 0, 1.3).withTarget(900, 80, -45, 1.3).drive().waitUntilSettled();
  roller(100);
  chassis.left(80);
  chassis.right(80);
  rack.move(RACK_UP,127,13).withTol(20);
  delay(500);
  chassis.left(0);
  chassis.right(0);
  rack.waitUntilSettled();
  roller(-50);
  delay(90);
  roller(-90);
  rack.move(RACK_DOWN,127,13).withTol(10);


  //do not touch ^^^^^^^
  //do not touch ^^^^^^^
  //do not touch ^^^^^^^
  //do not touch ^^^^^^^
  //do not touch ^^^^^^^
  //do not touch ^^^^^^^
  //do not touch ^^^^^^^
  //do not touch ^^^^^^^




  //chassis.drive(-400, 100,8).withTol(5).waitUntilSettled();
  roller(0);
  chassis.turn(150, 100,8).withTol(5).waitUntilSettled();
  arm.move(0.7, 127);
  chassis.lock();
  //chassis.withGyro(-85, 2).drive(-1650, 70, 3).waitUntilSettled();
  delay(200);
  chassis.turn(450, 100,8).waitUntilSettled();
  delay(200);
  chassis.unlock();



  //transitioon code ^^
  //transitioon code ^^
  //transitioon code ^^
  //transitioon code ^^
  //transitioon code ^^
  //transitioon code ^^
  //transitioon code ^^
  //transitioon code ^^


  chassis.left(-60);
  chassis.right(-60);
  delay(800);
  chassis.left(0);
  chassis.right(0);
  roller(0);
  chassis.calibrateGyro();
  delay(500);
  arm.zero().waitUntilSettled();
  roller(127);
  chassis.lock();
//  chassis.withGyro(0,2).drive(2750, 60, 6).waitUntilSettled();
  delay(300);
  arm.withTol(20).tower(1);
  delay(800);
  chassis.turn(-200, 100).withConst(0.6).waitUntilSettled();
//  chassis.withGyro(55,1).withTol(10).drive(200, 40, 3).waitUntilSettled();
  roller(-80);
  delay(1000);
  roller(0);
//  chassis.withGyro(55,1).withTol(10).drive(-180, 40, 3).waitUntilSettled();
  chassis.turn(250, 100).withConst(0.6).waitUntilSettled();
  arm.zero();
  roller(127);
  chassis.waitUntilSettled();
  arm.waitUntilSettled();
  //chassis.withGyro(0,2).drive(3100, 60, 1).withConst(0.6).waitUntilSettled();
  roller(-70);
  delay(500);
  roller(100);
  //chassis.withGyro(0,2).drive(-1200, 60, 1).waitUntilSettled();
  roller(70);
  chassis.turn(-200, 127,5).withTol(10).withConst(1.1).waitUntilSettled();
  //chassis.withGyro(45,2).drive(2300, 127, 5);
  delay(1500);
  rack.move(RACK_UP,127,13).withTol(20);
  chassis.waitUntilSettled();
  roller(-0.4,60);
  chassis.left(80);
  chassis.right(80);
  rack.waitUntilSettled();
  roller(-100);
  rack.move(RACK_DOWN,127,13).withTol(10);
  //chassis.withGyro(45,0).drive(-250, 100, 15).waitUntilSettled();
  //chassis.withGyro(45,2).drive(-1950, 100, 15).waitUntilSettled();

  //score one tower and stack 7 cubes
  //score one tower and stack 7 cubes
  //score one tower and stack 7 cubes
  //score one tower and stack 7 cubes
  //score one tower and stack 7 cubes
  //score one tower and stack 7 cubes
  //score one tower and stack 7 cubes

  roller(127);
  chassis.turn(600, 127,5).withTol(10).waitUntilSettled();
  //chassis.withGyro(-80,2).drive(4700, 50, 3).waitUntilSettled();
  //chassis.withGyro(-80,2).drive(-1100, 100, 3).waitUntilSettled();
  delay(700);
  arm.tower(1);
  delay(500);
  arm.move(1.15, 127);
  chassis.turn(340, 127,5).withTol(10).waitUntilSettled();
  //chassis.drive(440, 80, 3).waitUntilSettled();
  roller(-90);
  delay(400);
  //chassis.drive(-1200, 80, 3).waitUntilSettled();
  arm.zero();
  roller(127);
  delay(300);
  chassis.turn(-700, 127,5).withTol(10).waitUntilSettled();
  // chassis.withGyro(-45).drive(800, 80, 3).waitUntilSettled();
  chassis.left(50);
  chassis.right(50);
  delay(700);
  chassis.left(30);
  chassis.right(30);
  delay(400);
  roller(-0.3,60);
  rack.move(RACK_UP,127,13).withTol(20).waitUntilSettled();
  roller(-100);
  rack.move(RACK_DOWN,127,13).withTol(10);
  //chassis.drive(-700, 127, 5).waitUntilSettled();
}
