#include "main.h"

#include "control/drive.h"
#include "control/asyncController.h"
#include "control/macro.h"
#include "control/vision.h"
#include "control/path.h"

static Path path;
static ControlAsync Thread;
static Camera CamFront(FRONTVISION);

// Ignore below. Just for testing stuff.
void tester() {
  std::cout << "Testing" << std::endl;
  Thread.hold_angle();
}

/*===========================================
  PREMADE FUNCTIONS
===========================================*/
void start() {            // Deploy and zero arm
  lockChassis();
  arm(0.6,200,2);
  roller(-0.3,-200);
  wait(100);
  armReset();
  wait(100);
  unlockChassis();
}

/*===========================================
  RED MATCH AUTONOMOUSES
===========================================*/
void r_s_7() {        //red small 7
  // calibrategyro();
  // wait(500);
  start();
  roller(200);
  drive(1000, 100, 9);
  //pick up first 3 cubes
  //Thread.rack(RACK_TOWER,100,7);
  drive(-700, 100, 5);

  turn(280,100,3);
  //Thread.disable_rack();
  Thread.drive(-300, 200, 5);
  roller(-0.5,100);
  rack(RACK_UP, 150, 10);
  Thread.disable_drive();
  roller(-120);
  Thread.drive(300, 300, 10);
  rack(RACK_DOWN, 200, 15);


  // turn(100,400,9);
  //
  // drivegyro(950, 140, 8, 0 , 0.3 , 0);
  // drivegyro(-850, 200, 8, 0 , 0.3 , 10);
  // roller(-0.3,70);
  // Thread.rack(RACK_TOWER, 200, 2);
  // unlockChassis();
  // drivegyro(150,200,7,-130,1,20);
  // //driveinto corner
  // roller(-20);
  // Thread.disable_rack();
  // rack(RACK_UP, 200, 10);
  // //Thread.drive(10,200,9);
  // wait(100);
  // Thread.disable_drive();
  // roller(-120);
  // Thread.drive(300, 300, 10);
  // rack(RACK_DOWN, 200, 15);




  // drivegyro(850, 145, 7, 0,0.8,10);
  // //pick up 4 cubes
  // Thread.disable_arm();
  // drivegyro(-800,600,4,0,2,10);
  // //drive back to corner
  // lockChassis();
  // roller(-0.3,70);
  // Thread.rack(RACK_TOWER, 200, 2);
  // unlockChassis();
  // drivegyro(150,200,7,-130,1,20);
  // //driveinto corner
  // roller(-20);
  // Thread.disable_rack();
  // rack(RACK_UP, 200, 10);
  // //Thread.drive(10,200,9);
  // wait(100);
  // Thread.disable_drive();
  // roller(-120);
  // Thread.drive(300, 300, 10);
  // rack(RACK_DOWN, 200, 15);
}
void r_b_9() {       // red big 9
  calibrategyro();
  wait(800);
  drivegyro(350, 200, 7, 0, 1, 10);
  Thread.withGyro(0, 2).drive(-350, 200,9);
  wait(200);
  start();
  Thread.disable_drive();
  roller(200);

  drivegyro(600, 110, 7, 0, 1, 10);
  drivegyro(-100, 200, 7, 0, 1, 10);
  turn(500,200,7);
  drivegyro(250,100,6,-85,1,10);
  drivegyro(900,130,8,-0,1,10);
  drivegyro(700,200,1,100,1,10);
  drivegyro(700,100,2,90,1,10);

}

/*===========================================
  BLUE MATCH AUTONOMOUSES
===========================================*/
void b_s_7() {        //blue small 7
  calibrategyro();
  wait(800);
  start();
  roller(200);
  drivegyro(1100, 150, 9, 0 , 2 , 0);
  //pick up first 3 cubes
  drivegyro(-1280, 200, 15, -70 , 1 , 5);
  drivegyro(1200, 145, 3, 0,3,10);
  //pick up 4 cubes
  Thread.disable_arm();
  drivegyro(-1100,200,7,0,2,10);
  //drive back to corner
  lockChassis();
  roller(-0.7,70);
  Thread.rack(1700, 200, 10);
  unlockChassis();
  drivegyro(330,200,2,140,1,90);
  //driveinto corner
  //roller(-20);
  Thread.disable_rack();
  rack(RACK_UP, 190, 7);
  //Thread.drive(10,200,9);
  wait(100);
  Thread.disable_drive();
  roller(-200);
  wait(100);
  Thread.drive(-300, 200, 10);
  rack(RACK_DOWN, 200, 15);
}
void b_b_9() {        // blue big 9
  calibrategyro();
  wait(800);
  drivegyro(300, 200, 7, 0, 1, 10);
  Thread.withGyro(0, 2).drive(-300, 200,9);
  wait(100);
  start();
  Thread.disable_drive();
  roller(200);

  drivegyro(600, 110, 7, 0, 1, 10);
  drivegyro(-100, 200, 7, 0, 1, 10);
  turn(-500,200,7);
  drivegyro(300,100,3,85,1,10);
  drivegyro(1000,100,5,25,1,10);
  drivegyro(700,200,1,-100,1,10);
  drivegyro(600,100,2,-90,1,10);
}

/*===========================================
  SKILLS AUTONOMOUSES
===========================================*/
void skills1() {          // official skills
  calibrategyro();
  wait(1000);
//  drivegyro(300, 100, 1, 45, 1, 1700, 600, 10, 0, 0.1);

  wait(100000);
  calibrategyro();
  wait(1000);
  start();

  wait(100);

  //prime robot
  roller(200);


  drivegyro(1400,130,9,0,2,5);
   drivegyro(350, 160, 9, -10, 1, 15);
   drivegyro(350, 160, 9, 20, 1, 15);
   drivegyro(1100,130,3,5,1,0);
   drivegyro(-670,100,5,-100,1,10);
   roller(-200);
   wait(250);
   roller(0);
   rack(1500, 200, 11);
   Thread.rack(RACK_DOWN, 200, 15);
   roller(100);
   drivegyro(700,200,9,-90,1,10);
   roller(40);
   wait(200);
   roller(10);
   wait(200);
   tower(1);
   roller(-150);
   wait(400);
   drivegyro(-100,50,1,0,2,10);
   roller(100);
   armReset();
   wait(500);
   roller(-0.5,50);
   Thread.rack(1500, 200, 10);
   drivegyro(250,200,9,0,2,10);
   drivegyro(550,150,10,-55,0.8,50);
   Thread.disable_rack();
   roller(-25);
   rack(RACK_UP, 200, 10);
   roller(-200);
   wait(50);

  //score 7 cubes in small blue zone
  roller(-200);
  Thread.rack(RACK_DOWN, 200, 15);
  drive(-270, 200, 6);
  wait(100);
  turn(-700, 200, 9);
  align(300, 3);
  roller(0);
  Thread.arm(1, 100, 20);
  drive(1000,200,6);
  turn(-500, 150, 7);
  align(300, 3);
  Thread.disable_arm();
  armReset();
  roller(200);
  drive(1200,90,4);
  roller(100);
  wait(200);

  // Low
  tower(1);
  turn(-220, 150, 4);
  roller(-70);
  drive(150,150,8);

  // Drive to red tower and drop out cube
  drive(-350,150,4);
  roller(200);
  Thread.arm(ARM_BOTTOM-0.2, 200, 10);
  wait(400);
  drive(200,100,5);
  turn(230, 200, 6);
  roller(200);
  drive(1600,90,6);
  Thread.disable_arm();
  roller(100);

  wait(300);
  tower(2);

  // Lift arm to mid tower height
  turn(330,80,7);
  drive(240,150,5);
  roller(-160);

  // Spit cube into the mid tower
  wait(500);
  Thread.arm(ARM_BOTTOM-0.2, 200, 10);
  roller(200);

  drive(-700,200,5);
  wait(100);
  roller(200);
  roller(-0.6,-70);
  turn(-550, 150, 2);
  Thread.rack(RACK_TOWER-200, 100, 9);
  drive(980,180,9);
  roller(-20);
  Thread.disable_rack();
  rack(RACK_UP, 200, 9);
  Thread.disable_arm();

  Thread.disable_drive();
  roller(-100);
  Thread.rack(RACK_DOWN, 200, 15);
  wait(100);
  drive(-300, 200, 2);
}
