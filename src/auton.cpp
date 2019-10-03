#include "main.h"

#include "control/drive.h"
#include "control/asyncController.h"
#include "control/macro.h"
#include "control/vision.h"
#include "control/path.h"
using namespace path;

static ControlAsync Thread;
static Camera CamFront(FRONTVISION);
//begining of function library
void start() {  //prime robot for beginning
  roller(-3,200);
  armReset();
}
//end of functionlibrary

//match autonomaus library
void tester() {            //auton building
  std::cout << "Testing" << std::endl;
  CamFront.target(CUBE_PURPLE, 0, 0, 0, 1);
}

void b_s_8() {          //blue small 8 cube
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

void r_s_8() {           //red small 8 cube
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

void R_b_8_() {           // red big 8 cube

}

void b_b_8() {           // blue big 8 cube

}
//end of match autonomaus library

//skills autonomaus library
void skills1() {         // official skills
  start();
  //prime robot
  roller(100);
  drive(350, 100, 8);
  roller(0);
  Thread.arm(ARM_LOW_TOWER, 200, 60);
  //pick up one cube for small red tower
  turn(280, 150, 7);
  roller(-80);
  drive(280,150,8);
  //drive to red tower and drop out cube
  drive(-280,150,8);
  Thread.arm(ARM_BOTTOM - 0.2, 200, 20);
  turn(-275, 150, 7);
  roller(200);
  //return to primary position to grab three more cubes
  drive(920, 130, 8);
  Thread.disable_arm();
  tower(1);
  //arm up with macro to score mid tower
  turn(-150,100,5);
  roller(-100);
  wait(200);
  Thread.arm(ARM_BOTTOM - 0.3, 150, 5);
  turn(140,100,5);
  //return to primary positiono to grab 5 more cubes
  roller(200);

  //turn(20,100,5);
  drive(1100, 120, 5);
  wait(1000);
  Thread.disable_arm();
  Thread.turn(300, 150, 7);
  tower(1);
  roller(-100);
  drive(300,150,8);
  //drive to blue tower and drop out cube
  drive(-280,150,8);
  Thread.arm(ARM_BOTTOM - 0.2, 200, 5);
  turn(-300, 150, 7);
  roller(200);
  drive(700,100,5);
  turn(270,100,5);
  CamFront.target(BLUE_ZONE, 0, 0, 0, 1);

  Thread.disable_arm();
  roller(-20);
  Thread.drive(550, 50, 9);
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
  Thread.arm(ARM_MID_TOWER, 200, 20);
  drive(1050,200,6);
  Thread.disable_arm();
  roller(200);
  turn(-500, 150, 7);
  Thread.arm(ARM_BOTTOM-0.2, 200, 20);
  align(300, 2);
  drive(2800,100,6);
  wait(1000);
  Thread.disable_arm();
  tower(2);
  //lift arm to mid tower height
  turn(350,80,7);
  drive(180,150,5);
  roller(-160);
  //spit cube into the mid tower
  wait(900);
  drive(-600,200,5);
  Thread.disable_arm();
  armReset();
  roller(200);
  turn(-600, 150, 7);
  roller(200);
  drive(700,100,5);
  wait(400);
  Thread.drive(300,130,5);
  roller(-24);
  rack(RACK_UP, 110, 11);
  wait(100);
  roller(-100);
  Thread.rack(RACK_DOWN, 200, 15);
  drive(-300, 200, 9);
}
//end of skills autonomaus library
