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

void skills1() {
  // armReset();
  // tower(1);
  // wait(99999999);
  roller(-3,200);
  armReset();

  roller(100);
  drive(400, 100, 8);
  roller(0);
  Thread.arm(ARM_LOW_TOWER, 200, 20);
  drive(-30,150,8);
  turn(300, 150, 5);
  roller(-80);
  drive(250,150,8);
  drive(-250,150,8);
  Thread.arm(ARM_BOTTOM - 0.2, 150, 5);
  turn(-300, 150, 5);
  roller(200);
  drive(850, 100, 8);
  roller(100);
  Thread.disable_arm();
  wait(200);
  tower(1);
  wait(400);
  roller(0);
  wait(500);
  turn(-100,100,5);
  roller(-200);
  wait(4000);


  align(300, 5);

  drive(1000, 95, 9);
  turn(750, 90, 5);

  roller(-22);
  Thread.drive(1480, 160, 9);
  rack(RACK_UP, 100, 11);
  wait(400);

  // Yeet outta there

  Thread.drive(-300, 200, 9);
  roller(-100);
  rack(RACK_DOWN, 200, 15);
  turn(-720,80,6);
  drive(-200, 115, 9);

  armReset();

  align(300, 5);
  roller(200);
  turn(-190,80,7);
  drive(1200, 200, 9);
  turn(200,80,7);
  drive(1950, 140, 9);
  turn(305,100,9);
  drive(890, 200, 8);
  turn(650,100,9);
  drive(1000, 90, 5);
  turn(-780,200,5);
  roller(-28 );
  Thread.drive(900, 160, 9);
  rack(RACK_UP, 120, 11);
  roller(50);
  wait(200);

  // Yeet outta there
  Thread.drive(-300, 200, 9);
  roller(-100);
  rack(RACK_DOWN, 200, 15);
  turn(-720,80,6);
  drive(-200, 115, 9);

  armReset();
  align(300, 5);
  roller(200);
  drive(1000, 200, 5);
  wait(300);
  tower(2);
  roller(-200);
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
