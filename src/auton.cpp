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
  roller(-200);
  lockChassis();
  wait(50);
  roller(-200);
  arm(0.4, 100, 60);
  wait(50);
  roller(-200);
  //Thread.drive(-70,100,9);
  armReset();
  //Thread.drive(-70,150,9);
  wait(300);
  //Thread.disable_drive();
  unlockChassis();
}

/*===========================================
  RED MATCH AUTONOMOUSES
===========================================*/
void r_s_7() {

  calibrategyro();
  wait(1000);

  start();
  roller(200);
  drivegyro(1100, 150, 9, 0 , 2 , 0);
  //pick up first 3 cubes
  drivegyro(-1250, 200, 15, 65 , 0.05 , 5);
  drivegyro(1400, 135, 4, 0,3,10);
  //pick up 4 cubes
  Thread.disable_arm();
  drivegyro(-1050,200,4,0,2,10);
  //drive back to corner
  lockChassis();
  roller(-0.5,70);
  Thread.rack(1500, 200, 10);
  unlockChassis();
  drivegyro(400,200,2,-135,1,20);
  //driveinto corner
  roller(-20);
  Thread.disable_rack();
  rack(RACK_UP, 200, 10);
  drive(10,200,100);
  roller(-150);
  Thread.drive(-300, 200, 10);
  rack(RACK_DOWN, 200, 15);

}
void r_s_8() {            // red small 8 cube

  start();
  Thread.disable_drive();
  roller(200);
  // Pick up 4 cubes

  drive(1300, 160, 9, 10000, 0);

  // Yoink 3 more cubes and turn right facing small red corner
  //path.run("yeet", true);
  //path.del("yeet");

  drive(1400, 145, 9, 10000, 0);
  Thread.rack(RACK_TOWER-200, 120, 7);
  drive(-800, 200, 9, 10000, 0);
  Thread.disable_arm();
  roller(-20);
  turn(750, 200, 7);

  // Drive to small red corner and place
  roller(0);
  roller(-31);
  Thread.drive(550, 90, 6);
  rack(RACK_UP, 125, 11);
  Thread.disable_drive();
  wait(100);
  Thread.disable_rack();
  //score 7 cubes in small blue zone
  roller(-200);
  Thread.rack(RACK_DOWN, 200, 15);
  drive(-270, 200, 9);


}
void r_n_9() {            // Red No Score 9 Cube
  start();
  roller(200);
  // Pick up 4 cubes

  drive(1100, 105, 9, 10000, 0);


  // Yoink 3 more cubes and turn right facing small red corner
  //path.run("yeety", true);
  //path.del("yeety");

  drive(1200, 140, 9, 10000, 0);
  Thread.disable_arm();
  turn(750, 160, 7);

  // Drive to small red corner and place
   drive(1300, 200, 9);
   roller(-100);


  // Yeet outta there
  drive(-300, 200, 10);
  roller(0);



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
void b_s_7() {


    start();
    roller(200);
    // Pick up 4 cubes

    drive(1250, 145, 9, 10000, 0);

    // Yoink 3 more cubes and turn right facing small red corner
    //path.run("yote", true);
    //path.del("y0te");

    drive(1200, 145, 9, 10000, 0);
    Thread.disable_arm();
    turn(-690, 160, 7);

    // Drive to small red corner and place
    roller(-25);
    Thread.drive(1500, 150, 9);
    rack(RACK_UP, 100, 6);

    // Yeet outta there
    Thread.drive(-300, 200, 10);
    roller(-200);
    rack(RACK_DOWN, 200, 15);


}
void b_s_8() {            // blue small 8 cube
  start();
  roller(200);
  // Pick up 4 cubes

  drive(1100, 105, 9, 10000, 0);

  // Yoink 3 more cubes and turn right facing small red corner
//  path.run("yote", true);
//  path.del("yote");

  drive(1200, 140, 9, 10000, 0);
  Thread.disable_arm();
  turn(-700, 160, 7);

  // Drive to small red corner and place
  roller(-23);
  Thread.drive(1450, 150, 9);
  rack(RACK_UP, 95, 6);

  // Yeet outta there
  Thread.drive(-300, 200, 10);
  roller(-200);
  rack(RACK_DOWN, 200, 15);
}
void b_n_9() {            // Blue No Score 9 cube
  start();
  roller(200);
  // Pick up 4 cubes

  drive(1100, 105, 9, 10000, 0);


  // Yoink 3 more cubes and turn right facing small red corner
  //path.run("yote", true);
  //path.del("yote");

  drive(1200, 140, 9, 10000, 0);
  Thread.disable_arm();
  turn(-680, 160, 7);

  // Drive to small red corner and place
   drive(1300, 200, 9);
   roller(-100);


  // Yeet outta there
  drive(-300, 200, 10);
  roller(0);


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
  wait(300);
    //prime robot
    roller(100);
    drivegyro(330, 100, 8,0,2,10);
    roller(0);
    Thread.arm(ARM_LOW_TOWER, 200, 60);

    //pick up one cube for small red tower
    turn(290, 150, 5);
    roller(-80);
    drive(290,150,8);

    //drive to red tower and drop out cube
    drive(-250,150,6);
    turn(-290, 150, 5);
    armReset();
    drivegyro(1100,130,9,0,2,5);
    drivegyro(350, 160, 9, -10, 1, 15);
    drivegyro(350, 160, 9, 20, 1, 15);
    drivegyro(1200,120,5,0,1,0);
    roller(200);
    //turn(20,100,5);
    roller(10);
    wait(300);
    Thread.disable_arm();
    Thread.turn(400, 150, 1);
    tower(1);
    wait(400);
    Thread.disable_drive();
    roller(-100);
    drive(250,150,8);
    wait(200);

    //drive to blue tower and drop out cube
    drive(-250,150,8);
    armReset();
    roller(200);
    wait(50);
    turn(-270, 150, 7);
    roller(-0.7,50);
    Thread.rack(1500, 200, 10);
    drivegyro(180,200,9,0,2,10);
    drivegyro(600,150,10,-55,0.8,50);
    Thread.disable_rack();
    roller(-10);
    rack(RACK_UP, 200, 3);
    roller(-200);
    wait(100);
    Thread.rack(RACK_DOWN, 200, 15);
    drive(-270, 200, 9);
    turn(-700, 150, 7);
    align(300, 2);

    //prime for stage 2 by alligning to first mid tower
    roller(0);
    Thread.arm(1, 100, 20);
    drive(1050,200,6);
    turn(-500, 150, 7);
    align(300,3);
    Thread.disable_arm();
    armReset();
    roller(0);
    calibrategyro();
    wait(1000);
    roller(200);
    drivegyro(1000,140,9,0,1,20);
    roller(10);
    wait(200);
    tower(1);
    drivegyro(200,140,9,40,1,20);
    roller(-150);
    wait(400);
    drivegyro(-400,140,9,40,1,20);
    roller(200);
    armReset();
    drivegyro(2100,160,6,0,1.5,20);
    wait(500);
    tower(2);
    turn(350,80,7);
    drive(160,150,5);
    roller(-130);
    // Spit cube into the mid tower
    wait(500);
    roller(200);
    drive(-900,200,5);
    armReset();
    roller(200);
    wait(200);
    roller(10);
    wait(400);
    Thread.disable_arm();
    Thread.withGyro(90, 1).drive(200, 150, 3, 19);
    tower(1);
    wait(500);
    roller(-130);
    wait(500);
    drive(-200,200,9);
    drivegyro(-200, 200, 9,45,1,0);
    armReset();
    roller(200);
    wait(500);
    roller(-0.7,70);
    Thread.rack(RACK_TOWER, 200, 9);
    drivegyro(1000, 200, 9,45,1,0);
    roller(-10);
    Thread.disable_rack();
    rack(RACK_UP, 200, 10);
    roller(-200);
    wait(100);
    Thread.rack(RACK_DOWN, 200, 15);
    drive(-270, 200, 9);
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
    turn(240, 150, 4);
    roller(200);
    drive(1600,90,6);
    Thread.disable_arm();

    wait(500);
    tower(2);

    //lift arm to mid tower height
    turn(350,80,7);
    drive(180,150,5);
    roller(-160);
    //spit cube into the mid tower
    wait(500);
    drive(-600,200,5);
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


































drivegyro(1400,130,9,0,2,5);




  wait(10000);
  calibrategyro();
  wait(1000);
  start();

  wait(100);

  //prime robot
  roller(200);
  drivegyro(1400,130,9,0,2,5);
  drivegyro(350, 160, 9, -10, 1, 15);
  drivegyro(350, 160, 9, 20, 1, 15);
  drivegyro(1200,120,5,0,1,0);
  drivegyro(-650,100,5,-100,1,10);
  roller(-200);
  wait(250);
  roller(0);
  rack(1500, 200, 11);
  Thread.rack(RACK_DOWN, 200, 15);
  roller(100);
  drivegyro(650,200,9,-90,1,10);
  roller(40);
  wait(200);
  tower(1);
  roller(-150);
  wait(400);
  drivegyro(-100,50,1,0,2,10);
  roller(100);
  armReset();
  wait(500);
  roller(-0.7,50);
  Thread.rack(1500, 200, 10);
  drivegyro(180,200,9,0,2,10);
  drivegyro(600,150,10,-55,0.8,50);
  Thread.disable_rack();
  roller(-10);
  rack(RACK_UP, 200, 3);
  roller(-200);
  wait(100);
  Thread.rack(RACK_DOWN, 200, 15);
  drive(-270, 200, 9);
  wait(100);
  turn(-700, 200, 9);
  align(300, 3);
  roller(0);
  Thread.arm(1, 100, 20);
  drive(1000,200,6);
  turn(-500, 150, 7);
  align(300, 3);
  wait(200);
  align(300,3);
  Thread.disable_arm();
  armReset();
  roller(0);
  calibrategyro();
  wait(1000);
  roller(200);
  drivegyro(1000,140,9,0,1,20);
  roller(10);
  wait(200);
  tower(1);
  drivegyro(200,140,9,40,1,20);
  roller(-150);
  wait(400);
  drivegyro(-400,140,9,40,1,20);
  roller(200);
  armReset();
  drivegyro(2100,160,6,0,1.5,20);
  wait(500);
  tower(2);
  turn(350,80,7);
  drive(160,150,5);
  roller(-130);
  // Spit cube into the mid tower
  wait(500);
  roller(200);
  drive(-900,200,5);
  armReset();
  roller(200);
  wait(200);
  roller(10);
  wait(400);
  Thread.disable_arm();
  Thread.withGyro(90, 1).drive(200, 150, 3, 19);
  tower(1);
  wait(500);
  roller(-130);
  wait(500);
  drive(-200,200,9);
  drivegyro(-200, 200, 9,45,1,0);
  armReset();
  roller(200);
  wait(500);
  roller(-0.7,70);
  Thread.rack(RACK_TOWER, 200, 9);
  drivegyro(1000, 200, 9,45,1,0);
  roller(-10);
  Thread.disable_rack();
  rack(RACK_UP, 200, 10);
  roller(-200);
  wait(100);
  Thread.rack(RACK_DOWN, 200, 15);
  drive(-270, 200, 9);
  wait(10000);






  rack(RACK_UP, 110, 11);
  Thread.disable_arm();

  Thread.disable_drive();
  roller(-100);
  Thread.rack(RACK_DOWN, 200, 15);
  wait(100);
  drive(-300, 200, 2);




















  wait(10000);
  strafe(-80,100,5);
  drive(1200, 130, 5);
  strafe(50,100,5);
  drive(600,100,5);
  turn(520, 150, 2);
  drive(-600,100,5);
  roller(-200);
  wait(250);
  roller(0);
  Thread.drive(550,130,7);
  roller(100);
  rack(1500, 200, 11);
  roller(200);
  wait(200);
  Thread.disable_drive();
  Thread.rack(RACK_DOWN, 200, 15);
  roller(100);
  turn(70, 200, 6);
  wait(200);
  roller(10);
  wait(200);
  Thread.disable_drive();

  tower(1);
  drive(250,100,5);
  roller(-150);
  wait(400);
  Thread.drive(-250,100,5);
  armReset();
  Thread.disable_drive();
  roller(200);
  Thread.disable_rack();
  turn(-380,150,5);
  roller(0);
  roller(-31);
  Thread.drive(600, 80, 6);
  rack(RACK_UP, 135, 11);
  Thread.disable_drive();
  wait(300);
  Thread.disable_rack();

  //score 7 cubes in small blue zone
  roller(-200);
  Thread.rack(RACK_DOWN, 200, 15);
  drive(-270, 200, 9);
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
  drive(-150,150,4);
  roller(200);
  Thread.arm(ARM_BOTTOM-0.2, 150, 10);
  turn(230, 150, 4);
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
  turn(-550, 150, 7);
  Thread.drive(1000,90,5);
  roller(-30);
  rack(RACK_UP, 110, 11);
  Thread.disable_arm();

  Thread.disable_drive();
  roller(-100);
  Thread.rack(RACK_DOWN, 200, 15);
  wait(100);
  drive(-300, 200, 2);






  // start();
  //
  // wait(100);
  //
  // //prime robot
  // roller(200);
  // drive(1300,130,5);
  // strafe(-80,100,5);
  // drive(1200, 130, 5);
  // strafe(50,100,5);
  // drive(600,100,5);
  // turn(520, 150, 2);
  // drive(-600,100,5);
  // roller(-200);
  // wait(250);
  // roller(0);
  // Thread.drive(550,130,7);
  // roller(100);
  // rack(1500, 200, 11);
  // roller(200);
  // wait(200);
  // Thread.disable_drive();
  // Thread.rack(RACK_DOWN, 200, 15);
  // roller(100);
  // turn(70, 200, 6);
  // wait(200);
  // roller(10);
  // wait(200);
  // Thread.disable_drive();
  //
  // tower(1);
  // drive(250,100,5);
  // roller(-150);
  // wait(400);
  // Thread.drive(-250,100,5);
  // armReset();
  // Thread.disable_drive();
  // roller(200);
  // Thread.disable_rack();
  // turn(-380,150,5);
  // roller(0);
  // roller(-31);
  // Thread.drive(600, 80, 6);
  // rack(RACK_UP, 135, 11);
  // Thread.disable_drive();
  // wait(300);
  // Thread.disable_rack();
  //
  // //score 7 cubes in small blue zone
  // roller(-200);
  // Thread.rack(RACK_DOWN, 200, 15);
  // drive(-270, 200, 9);
  // wait(100);
  // turn(-700, 200, 9);
  // align(300, 3);
  // roller(0);
  // Thread.arm(1, 100, 20);
  // drive(1000,200,6);
  // turn(-500, 150, 7);
  // align(300, 3);
  // Thread.disable_arm();
  // armReset();
  // roller(200);
  // drive(1200,90,4);
  // roller(100);
  // wait(200);
  //
  // // Low
  // tower(1);
  // turn(-220, 150, 4);
  // roller(-70);
  // drive(150,150,8);
  //
  // // Drive to red tower and drop out cube
  // drive(-150,150,4);
  // roller(200);
  // Thread.arm(ARM_BOTTOM-0.2, 150, 10);
  // turn(230, 150, 4);
  // roller(200);
  // drive(1600,90,6);
  // Thread.disable_arm();
  // roller(100);
  //
  // wait(300);
  // tower(2);
  //
  // // Lift arm to mid tower height
  // turn(330,80,7);
  // drive(240,150,5);
  // roller(-160);
  //
  // // Spit cube into the mid tower
  // wait(500);
  // Thread.arm(ARM_BOTTOM-0.2, 200, 10);
  // roller(200);
  //
  // drive(-700,200,5);
  // wait(100);
  // roller(200);
  // turn(-550, 150, 7);
  // Thread.drive(1000,90,5);
  // roller(-30);
  // rack(RACK_UP, 110, 11);
  // Thread.disable_arm();
  //
  // Thread.disable_drive();
  // roller(-100);
  // Thread.rack(RACK_DOWN, 200, 15);
  // wait(100);
  // drive(-300, 200, 2);
}
