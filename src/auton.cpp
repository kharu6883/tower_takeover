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
   chassis.withGyro(-131,0.7).drive(2100, 100,1).waitUntilSettled();
   roller(-0.3,70);
   rack.move(RACK_UP,127,13).withTol(10).waitUntilSettled();
   roller(-100);
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
   chassis.unlock();
   chassis.withGyro(0,0.7).drive(-1400, 127,1).waitUntilSettled();
   chassis.withGyro(105,2).drive(1400, 127,1).waitUntilSettled();

   roller(-0.3,70);
   rack.move(RACK_UP,127,13).withTol(10).waitUntilSettled();
   roller(-100);
   chassis.drive(-500, 127,5).withTol(5).waitUntilSettled();

   io::master.rumble(" .");

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
  chassis.withGyro(-35, 0.5).drive(-900, 100, 9);
  arm.zero().waitUntilSettled();
  chassis.waitUntilSettled();
  delay(300);
  chassis.unlock();
  chassis.waitUntilSettled();

  chassis.withTarget(1500, 60, 0, 1.3,1).withTarget(400, 60, -35, 0.5).withTarget(800, 60, 0, 1.3).withTarget(600, 60, 35, 0.5).withTarget(2450, 60, 0, 1.3).withTarget(900, 60, -45, 1.3).drive().waitUntilSettled();
  roller(100);
  rack.move(RACK_UP,127,13).withTol(10).waitUntilSettled();
  roller(-120);
  rack.move(RACK_DOWN,127,13).withTol(10);

  chassis.drive(-300, 100,8).withTol(5).waitUntilSettled();
  chassis.turn(140, 100,8).withTol(5).waitUntilSettled();

  chassis.drive(-1400, 80,6).withTol(5).waitUntilSettled();
  chassis.turn(500, 100,8).waitUntilSettled();
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
  delay(800);
  chassis.withGyro(55,1).withTol(10).drive(500, 40, 3).waitUntilSettled();
  roller(-80);
  delay(1000);
  roller(0);
  chassis.withTarget(-400, 100,0,0.5,6).drive();
  delay(700);
  arm.zero();
  roller(127);
  chassis.waitUntilSettled();
  arm.waitUntilSettled();
  chassis.withGyro(0,2).drive(3500, 60, 1).waitUntilSettled();
  chassis.withGyro(0,2).drive(-1000, 60, 1).waitUntilSettled();
  roller(127);
  chassis.turn(-200, 127,5).withTol(10).waitUntilSettled();
  chassis.withGyro(45,2).drive(1500, 127, 3).waitUntilSettled();
  roller(-0.5,60);
  rack.move(RACK_UP,127,13).withTol(20).waitUntilSettled();
  roller(-100);
  rack.move(RACK_DOWN,127,13).withTol(10);
  chassis.drive(-500, 127,5).withTol(5).waitUntilSettled();






  delay(100000);
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








































  delay(100000);
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
  chassis.withGyro(-30, 0.5).drive(-750, 100, 6);
  arm.zero().waitUntilSettled();
  chassis.waitUntilSettled();
  delay(300);
  chassis.unlock();
  chassis.waitUntilSettled();
  //re align to row of 9 cubes
  chassis.withGyro(0,2).drive(1700, 50).waitUntilSettled();
  delay(500);
  arm.tower(1);
  delay(1000);
  arm.move(1.05, 127);
  chassis.lock();
  chassis.withGyro(20, 1).drive(400, 70, 3).waitUntilSettled();
  //deploy, drive to tower

  arm.waitUntilSettled();
  roller(-80);
  delay(600);
  //score red tower

  roller(127);
  chassis.withGyro(0, 0.5).drive(-1800, 100, 6);
  delay(300);
  arm.zero().waitUntilSettled();
  chassis.waitUntilSettled();
  chassis.withGyro(90,2).drive(1300, 100,1);
  delay(500);
  arm.tower(2);
  delay(300);
  arm.move(1.6,127);
  chassis.waitUntilSettled();
  delay(500);
  roller(-100);
  arm.move(0);
  delay(600);
    chassis.withGyro(0,2).drive(-200, 50,3).waitUntilSettled();
    arm.zero().waitUntilSettled();
    roller(127);
    chassis.withGyro(0,2).drive(5000, 100,3).waitUntilSettled();
    delay(200);
    arm.tower(2);
    delay(300);
    arm.move(1.6,127);
    chassis.withGyro(45,2).drive(500, 100,3).waitUntilSettled();
    arm.move(0);
    roller(-100);
  delay(10000);
  arm.zero();
  delay(300);
  chassis.unlock();
  chassis.waitUntilSettled();
  chassis.turn(300, 100, 7).withTol(10).waitUntilSettled();
  chassis.withTarget(200, 30, 0, 2).withTarget(400, 50, -35, 0.5).withTarget(400, 127, 0, 1.3).withTarget(600, 60, 35, 0.5).withTarget(550, 30, 0, 1.3).drive().waitUntilSettled();
  //multipoint drive to blue small corner with 9 cubes
  delay(900);
  arm.tower(1);
  delay(500);
  arm.move(1.05, 127);
  chassis.lock();
  chassis.withGyro(-50, 1).drive(650, 70, 3).waitUntilSettled();
  //deploy, drive to tower

  arm.waitUntilSettled();
  roller(-60);
  delay(600);
  //score red tower

  roller(127);
  chassis.withGyro(-70, 1).drive(-750, 100, 6).waitUntilSettled();
  arm.zero();
  delay(300);
  chassis.unlock();
  chassis.waitUntilSettled();
  chassis.withTarget(300, 60, 40).withTarget(1600, 60, 0).drive().waitUntilSettled();
  chassis.turn(200, 100, 7).waitUntilSettled();
  chassis.withGyro(-45,2).drive(300, 100);
  roller(-0.5,60);
  delay(500);
  rack.move(RACK_UP,127,13).withTol(20).waitUntilSettled();
  chassis.setMode(IDLE);
  chassis.clearArr();
  roller(-100);
  rack.move(RACK_DOWN,127,13);
  chassis.withTarget(-500, 100, 0, 0,6).withTarget(-500, 80, -90).drive().waitUntilSettled();
  chassis.turn(300,70,5).waitUntilSettled();

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
  chassis.withTarget(2000, 80, 0).withTarget(1000, 127, 0, 2, 4).withTarget(2000, 80, 0).drive().waitUntilSettled();

  chassis.withGyro(0,2).drive(-600, 60, 1).waitUntilSettled();
  chassis.turn(300, 127,1).withTol(7).waitUntilSettled();
  //grab 3 cubes

  chassis.drive(-200, 100);
  arm.tower(2);
  chassis.waitUntilSettled();
  delay(400);
  chassis.drive(400, 100);
  arm.move(1.5, 127);
  chassis.waitUntilSettled();
  roller(-80);
  delay(500);
  chassis.drive(-200,50).waitUntilSettled();
  arm.zero();
  chassis.turn(-300, 127,1).withTol(7).waitUntilSettled();
  chassis.drive(-500, 100).waitUntilSettled();
  chassis.turn(400, 127,1).withTol(7).waitUntilSettled();
  chassis.withTarget(1000, 127, -90).withTarget(2000, 80, -90, 2, 4).withTarget(-5000, 127, -90).drive().waitUntilSettled();
  chassis.left(-80);
  chassis.right(-80);
  delay(700);
  chassis.left(0);
  chassis.right(0);
  roller(0);
  chassis.calibrateGyro();
  delay(500);
  chassis.drive(300, 100).waitUntilSettled();
  chassis.turn(-300, 127,1).withTol(7).waitUntilSettled();
  chassis.drive(300, 100).waitUntilSettled();
  chassis.turn(-100, 127,1).withTol(7).waitUntilSettled();
  chassis.withGyro(115,2).drive(600, 60, 1);
  roller(-0.5,60);
  delay(500);
  rack.move(RACK_UP,127,13).withTol(20).waitUntilSettled();
  roller(-100);
  rack.move(RACK_DOWN,127,13).withTol(10);
  chassis.drive(-500, 127,5).withTol(5).waitUntilSettled();




  delay(10000);



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
  delay(500);
  roller(-0.8,100);
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
