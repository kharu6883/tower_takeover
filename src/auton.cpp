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
  path.withPoint({3_ft, 3_ft, 0_deg}).withPoint({6_ft, 1.5_ft, 0_deg}).gen("yeet");
  path.run("yeet", false);
  path.del("yeet");
}

/*===========================================
  PREMADE FUNCTIONS
===========================================*/
void start() {// Deploy and zero arm
  lockChassis();
  roller(-200);
  roller(-200);
  roller(-200);
  arm(0.4, 100, 60);
  roller(-200);
  armReset();
  wait(300);
  unlockChassis();
}


/*===========================================
  RED MATCH AUTONOMOUSES
===========================================*/
void r_s_8() {

  drive(220,200,10);
  Thread.drive(-260,200,10);         // red small 8 cube
  start();
  Thread.disable_drive();
  roller(250);
  // Pick up 4 cubes

  drive(1000, 95, 9, 10000, 0);

  // Yoink 3 more cubes and turn right facing small red corner
  path.run("yeety", true);
  path.del("yeety");

  drive(1200, 150, 9, 10000, 0);
  Thread.disable_arm();
  turn(770, 160, 7);

  // Drive to small red corner and place
  roller(-25);
  Thread.drive(1400, 155, 9);
  rack(RACK_UP, 105, 8);

  // Yeet outta there
  Thread.drive(-300, 200, 10);
  roller(-200);
  rack(RACK_DOWN, 200, 15);
}
void r_b_8() {            // red big 8 cube
  start();
  // Yoink cubes
  roller(200);
  drive(1600, 200, 7);

  // Back up, turn to cubes and yoink
  drive(-1050, 200, 7);
  turn(250, 150, 7);
  drive(1000, 160, 7);
  turn(-250, 100, 7);
  drive(100, 160, 7);
  turn(-700, 100, 7);

}
void r_b_orange() {       // red big orange cube
  start();
  roller(100);
  drive(200,100,50);
  roller(0);
  turn(-150,100,8);
  Thread.arm(ARM_LOW_TOWER, 200, 60);
  drive(1200,200,3);
  turn(300,100,8);
  drive(200,200,3);
  roller(-200);
  drive(-250,200,6);
  turn(320,50,6);
  Thread.disable_arm();
  roller(0);
}

/*===========================================
  BLUE MATCH AUTONOMOUSES
===========================================*/
void b_s_8() {            // blue small 8 cube
  start();
  // Pick up 4 cubes
  roller(200);
  drive(1000, 100, 9, 10000, 0);

  // Yoink 3 more cubes and turn right facing small red corner
  path.run("yote", true);
  path.del("yote");

  drive(1200, 140, 9, 10000, 0);
  Thread.disable_arm();
  turn(-700, 160, 7);

  // Drive to small red corner and place
  roller(-25);
  Thread.drive(1400, 155, 9);
  rack(RACK_UP, 105, 8);

  // Yeet outta there
  Thread.drive(-300, 200, 10);
  roller(-200);
  rack(RACK_DOWN, 200, 15);
}
void b_b_8() {            // blue big 8 cube

}
void b_b_green() {        // blue big green cube
  start();
  roller(100);
  drive(200,100,50);
  roller(0);
  turn(150,100,8);
  Thread.arm(ARM_LOW_TOWER, 200, 60);
  drive(1200,200,3);
  turn(-300,100,8);
  drive(200,200,3);
  roller(-200);
  drive(-250,200,6);
  turn(-320,50,6);
  Thread.disable_arm();
  roller(0);
}

/*===========================================
  SKILLS AUTONOMOUSES
===========================================*/
void skills1() {          // official skills

  start();
  unlockChassis();
  Thread.drive(-90,100,10);
  wait(300);
  Thread.disable_drive();
  wait(100);

  //prime robot
  roller(200);
  drive(1300,125,5);
  turn(140,100,5);
  drive(300,125,5);
  turn(-140,80,5);
  drive(400,100,6);
  turn(-140,80,5);
  drive(200,100,6);
  turn(140,80,5);
  drive(200, 125, 5);
  //turn(20,100,5);
  roller(100);
  wait(200);
  roller(50);
  wait(400);
  Thread.disable_arm();
  Thread.turn(290, 150, 2);
  tower(1);
  wait(300);
  Thread.disable_drive();
  drive(300,190,10);
    roller(-180);
  wait(300);

  //drive to blue tower and drop out cube
  drive(-300,150,8);
  Thread.arm(ARM_BOTTOM - 0.2, 200, 20);
  turn(-290, 150, 7);
  roller(200);
  drive(650,120,5);
  turn(200,150,9);

  CamFront.target(BLUE_ZONE, 0, 0, 0, 1);
  Thread.disable_arm();
  roller(-25);
  Thread.drive(700, 60, 9);
  rack(RACK_UP, 100, 11);
  Thread.disable_drive();
  wait(600);
  //score 7 cubes in small blue zone
  roller(-100);
  Thread.rack(RACK_DOWN, 200, 15);
  drive(-270, 200, 9);
  wait(100);
  turn(-700, 150, 7);
  align(300, 3);

  //prime for stage 2 by alligning to first mid tower
  roller(0);
  Thread.arm(1, 100, 20);
  drive(1000,200,6);
  turn(-500, 150, 7);
  align(300, 2);
  Thread.disable_arm();
  armReset();
  roller(200);
  drive(1200,90,6);
  roller(100);
  wait(400);
  tower(1);
  turn(-240, 150, 4);
  roller(-80);
  drive(150,150,8);

  //drive to red tower and drop out cube
  drive(-150,150,4);
  Thread.arm(ARM_BOTTOM - 0.2, 200, 20);
  turn(250, 150, 4);
  roller(200);
  drive(1600,90,6);
  Thread.disable_arm();

  wait(500);
  tower(2);

  //lift arm to mid tower height
  turn(350,80,7);
  drive(220,150,5);
  roller(-160);
  //spit cube into the mid tower
  wait(500);
  drive(-640,200,5);
  Thread.disable_arm();
  armReset();
  roller(200);
  turn(-580, 150, 7);
  Thread.drive(1070,100,5);
  roller(-26);
  rack(RACK_UP, 110, 11);
  Thread.disable_drive();
  wait(100);
  roller(-100);
  Thread.rack(RACK_DOWN, 200, 15);
  drive(-300, 200, 9);
}
