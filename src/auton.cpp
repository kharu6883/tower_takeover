#include "main.h"

#include "control/drive.h"
#include "control/asyncController.h"
#include "control/macro.h"
#include "control/vision.h"
#include "control/path.h"
using namespace path;

static ControlAsync Thread;
static Camera CamFront(FRONTVISION);

void tester() {
  std::cout << "Testing" << std::endl;
  CamFront.target(CUBE_PURPLE, 0, 0, 0, 1);
}

void start() {
  roller(-2,200);
  rack(2000, 200, 9);
  roller(0);
  rack(950, 200, 9);
  armReset();
}

void b_s_8() {
  roller(-3,200);
  armReset();

  generate("yoink", -3.3_ft, 3_ft, 1_deg);
  // Pick up 4 cubes
  roller(200);
  wait(100);
  drive(1000, 105, 9);

  // Swerve back to the 3 cubes
  execute("yoink", true);
  destroy("yoink");
  align(300, 5);

  // Yoink 3 more cubes and turn right facing small red corner
  drive(1000, 100, 9);
  turn(-730, 95, 5);

  // Drive to small blue corner and place
  roller(-23);
  Thread.drive(1440, 160, 9);
  rack(RACK_UP, 100, 11);
  wait(100);

  // Yeet outta there

  Thread.drive(-300, 200, 9);
  roller(-100);
  rack(RACK_DOWN, 200, 15);
}

void r_s_8() {
  roller(-3,200);
  armReset();

  generate("yoink", 3_ft, 3_ft, 1_deg);
  // Pick up 4 cubes
  roller(200);
  wait(100);
  drive(1000, 110, 9);

  // Swerve back to the 3 cubes
  execute("yoink", true);
  destroy("yoink");
  align(300, 5);

  // Yoink 3 more cubes and turn right facing small red corner
  drive(1000, 100, 9);
  turn(750, 95, 5);

  // Drive to small red corner and place
  roller(-24);
  Thread.drive(1480, 160, 9);
  rack(RACK_UP, 100, 11);
  wait(200);

  // Yeet outta there

  Thread.drive(-300, 200, 9);
  roller(-100);
  rack(RACK_DOWN, 200, 15);
}

void r_b_8_() {

}

void b_b_8() {

}

void skills1() {
  roller(-3,200);
  armReset();
  //primerobot
  roller(100);
  drive(350, 100, 8);
  roller(0);
  Thread.arm(ARM_LOW_TOWER, 200, 60);
  //pick up one cube for small red tower
  turn(270, 150, 7);
  roller(-80);
  drive(280,150,8);
  //drive to red tower and drop out cube
  drive(-280,150,8);
  Thread.arm(ARM_BOTTOM - 0.2, 200, 20);
  turn(-270, 150, 7);
  roller(200);
  //return to primary position to grab three more cubes
  drive(920, 100, 8);
  Thread.disable_arm();
  tower(1);
  //arm up with macro to score mid tower
  turn(-150,100,5);
  roller(-200);
  wait(300);
  Thread.arm(ARM_BOTTOM - 0.2, 150, 5);
  turn(140,100,5);
  //return to primary positiono to grab 5 more cubes
  roller(200);
  drive(1800, 130, 5);
  turn(230,100,5);
  Thread.disable_arm();
  roller(-24);
  Thread.drive(500, 160, 9);
  rack(RACK_UP, 100, 11);
  //score 7 cubes in small blue zone
  wait(100);
  roller(-100);
  Thread.rack(RACK_DOWN, 200, 15);
  drive(-200, 200, 9);
  turn(-700, 150, 7);
  align(300, 2);
  //prime for stage 2 by alligning to first mid tower
  roller(0);
  drive(850,200,6);
  CamFront.target(CUBE_PURPLE, 0, 0, 0, 1);
  //align to mid tower using purple cube at bottom of tower
  roller(200);
  drive(300,100,6);
  roller(0);
  Thread.drive(-60,150,5);
  arm(ARM_MID_TOWER, 100, 8);
  drive(100,150,5);
  roller(-130);
  wait(500);
  drive(-60,150,5);
  //spit cube into tower
  Thread.disable_arm();
  turn(-470, 150, 7);
  align(300, 3);
  //align to prime 3 stage of auton// drive and grab 6-8 cubes
  armReset();
  roller(200);
  drive(2700,150,6);
  wait(500);
  tower(2);
  //lift arm to mid tower height
  turn(320,80,7);
  drive(200,150,5);
  roller(-160);
  //spit cube into the mid tower
}

void skills2() {
  // Pick up 4 cubes
  roller(200);
  drive(1200, 50, 9);
  roller(0);

  // Score low tower
  wait(50);
  Thread.strafe(-380, 100, 5);
  tower(1);
  roller(-2, 150);
  wait(500);
  roller(200);

  // Approach red alliance tower and score
  Thread.drive(-600, 100, 9);
  tower(1);
  turn(400, 100, 9);
  drive(380, 100, 9);
  roller(-2, 180);

  // Drive to the wall, line up
  arm(0, 200, 9);
  generate("wallboi", -2.6_ft, 2_ft, -90_deg);
  execute("wallboi", true);
  destroy("wallboi");
  align(300, 3);

  // Yoink 9 cubes
  roller(200);
  drive(1800, 60, 5);
  turn(700, 50, 5);
  Thread.drive(380, 100, 9);
  roller(0);
  rack(3615, 60, 5);
  rack(950, 200, 15);

}

void motionTest() {

}
