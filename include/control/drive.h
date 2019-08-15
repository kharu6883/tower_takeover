#pragma once

/*--------------------------------
    DRIVE FUNCTIONS
--------------------------------*/

/* This section is only for driving functions. Movement functions such as operating the lift should not exist here. */

// Drives the robot in a straight line to the target in rotations, in a given speed(rpm). Uses PD loop. Rate controls the slew constant.
void drive(double target, int speed, double rate);

// Turns the robot to the target in rotations, in a given speed(rpm). Uses PD looop. Rate controls the slew constant.
void turn(double target, int speed, double rate);

// Strafes the robot to the target in ticks, in a given speed(rpm).
void strafe(double target, int speed, double rate);

// Strafes the robot to the target in ticks, in a given speed(rpm). Sturn yeet
void strafe(double target, int speed, double rate, double sturn);

// Lines upto the wall using the two ultrasonic sensors, to a given distance(cm).
void align(double target, double tolerance);

// Ignore the below functions. They accomodate the functions above.

void left(int speed);
void right(int speed);
