#include "main.h"
using namespace path;

// 6 = tolerance

ControlAsync Thread;

void tester() {
  std::cout << "Testing" << std::endl;

  Thread.drive(1000, 100, 1);

  // generate("1", 1_ft, 0_ft, 0_deg);
  // execute("1", false);
  // destroy("1");
  // generate("2", 0.6_ft, 0_ft, 0_deg);
  // execute("2", false);
  // destroy("2");
  // generate("3", 0.6_ft, 0_ft, 0_deg);
  // execute("3", false);
  // destroy("3");
  // generate("4", 0.6_ft, 0_ft, 0_deg);
  // execute("4", false);
  // destroy("4");
  // generate("5", 3_ft, 3_ft, 1_deg);
  // execute("5", true);
  // destroy("5");
  // generate("6", 1_ft, 0_ft, 0_deg);
  // execute("6", false);
  // destroy("6");
  // generate("7", 0.6_ft, 0_ft, 0_deg);
  // execute("7", false);
  // destroy("7");
}

void skills1() {
  // Pick up 4 cubes
  roller(200);
  drive(300, 150, 9);
  drive(700, 30, 9);
  roller(50);

  // Swerve back to the 3 cubes
  generate("yoink", 3_ft, 3_ft, 1_deg);
  execute("yoink", true);
  destroy("yoink");
  turn(20, 60, 9);

  // Yoink 3 more cubes
  roller(200);
  drive(1000, 35, 9);

  // Drive back and turn right facing small red corner
  drive(-1100, 85, 9);
  turn(530, 100, 9);

  // Small red corner and place cubes
  drive(900, 80, 9);
  wait(500);
  roller(-15);
  rack(3615, 90, 9);
  drive(50, 70, 15);

  // Escape the area like the wind
  Thread.drive(-950, 100, 9);
  roller(-200);
  rack(900, 150, 9);

  // // Small red corner and place cubes
  // drive(-500,200,9);
  // turn(750, 80, 9);
  // drive(455, 150, 5);
  // roller(-15);
  // rack(3.8, 90, 15);
  //
  // // Drive back
  // roller(-200);
  // drive(-300, 150, 9);
  // roller(0);
  // rack(0, 100, 20);
  //
  // // Middle tower
  // turn(720, 100, 9);
  // roller(100);
  // drive(930, 200, 9);
  // wait(1000);
  // drive(-80, 200, 9);
  // tower(2);
  //
  // // Turn towards and pick up 1 cube
  // roller(200);
  // turn(450, 80, 5);
  // drive(200, 80, 9);
  // drive(-400, 200, 9);
  //
  // // Towards red tower and score
  // turn(380, 80, 5);
  // drive(950, 200, 9);
  // turn(80,30,4);
  // tower(1);
  //
  // // Yoink one cube and score blue tower
  // roller(200);
  // turn(-500, 100, 5);
  // drive(800, 200, 9);
  // wait(1000);
  // drive(-50, 200, 9);
  // tower(1);
  //
  // // Yoink more cubes
  // turn(100,50,6);
  // roller(200);
  // drive(200,200,6);
  // turn(150,50,6);
  // drive(250,200,6);
  // turn(-200,80,6);
  // drive(200,200,9);
  // drive(1000,30,9);
  // drive(200,150,9);
  // turn(200,40,5);
  // drive(600,110,6);
  // roller(-15);
  // rack(3.8, 90, 9);
  // roller(0);
  //
  // // Drive back
  // roller(-200);
  // drive(-250, 150, 9);
  // roller(0);
  // rack(0, 100, 9);
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
