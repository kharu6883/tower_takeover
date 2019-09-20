#include "main.h"

#include "control/drive.h"
#include "control/asyncController.h"
#include "control/macro.h"
#include "control/path.h"
using namespace path;

static ControlAsync Thread;

void tester() {
  std::cout << "Testing" << std::endl;
  Thread.arm(ARM_LOW_TOWER_DESCORE, 200, 9);
}

void start() {
  roller(-2,200);
  rack(2000, 200, 9);
  roller(0);
  rack(950, 200, 9);
  armReset();
}

void r_s_8() {
  wait(5000);
  roller(-3,200);
   armReset();
  //
   generate("yoink", 3_ft, 3_ft, 1_deg);
   // Pick up 4 cubes
   roller(200);
   drive(1000, 115, 9);

   // Swerve back to the 3 cubes
   execute("yoink", true);
   destroy("yoink");
   align(300, 5);

   // Yoink 3 more cubes and turn right facing small red corner
   drive(1000, 95, 9);
  turn(750, 90, 5);

   // Drive to small red corner and place
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
  turn(-170,80,7);
  drive(400, 200, 9);
  turn(170,80,7);
  drive(700, 140, 9);
  drive(-160,70,9);
  roller(-10);
  wait(700);
  tower(1);
  drive(160,70,9);
  turn(-80,80,7);
  roller(-200);
  wait(500);
  turn(-200,80,7);
  armReset();
  roller(200);
  drive(500,70,6);
  wait(100000);
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

void skills1() {
  roller(-3,200);
  armReset();
 //
  generate("yoink", 3_ft, 3_ft, 1_deg);
  // Pick up 4 cubes
  roller(200);
  drive(1000, 115, 9);

  // Swerve back to the 3 cubes
  execute("yoink", true);
  destroy("yoink");
  align(300, 5);

  // Yoink 3 more cubes and turn right facing small red corner
  drive(1000, 95, 9);
 turn(750, 90, 5);

  // Drive to small red corner and place
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

  // wait(500);
  // roller(50);
  // turn(200, 100, 9);
  // drive(380, 100, 9);
  // roller(200);
  // wait(2000);
  // turn(-150, 100, 9);
  // Thread.drive(400, 100, 4);
  // roller(-10);
  // rack(3615, 60, 5);
}

void motionTest() {
  std::cout << "Reverse Traverse" << std::endl;
  generate("1", 5.5_ft, -1.5_ft, 0_deg);
  execute("1", false);
  destroy("1");

  generate("2", 5_ft, 4.5_ft, 90_deg);
  execute("2", false);
  destroy("2");

  generate("3", 2_ft, 2_ft, 100_deg);
  execute("3", false);
  destroy("3");

  generate("4", 4.5_ft, 4_ft, 0_deg);
  execute("4", false);
  destroy("4");

  generate("5", 5_ft, -4_ft, -10_deg);
  execute("5", false);
  destroy("5");

  generate("5", 9_ft, 0.3_ft, 0_deg);
  execute("5", true);
  destroy("5");

  generate("6", 5_ft, 3_ft, 0_deg);
  execute("6", false);
  destroy("6");

  generate("6", 4_ft, 1_ft, 0_deg);
  execute("6", false);
  destroy("6");

  generate("7", 0_ft, 0_ft, 90_deg);
  execute("7", false);
  destroy("7");
}
