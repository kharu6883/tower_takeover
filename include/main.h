#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

#define PROS_USE_SIMPLE_NAMES

#define PROS_USE_LITERALS

// Nice includes below
#include "api.h"
#include "pros/apix.h"
#include "pros/rtos.h"
#include "okapi/api.hpp"

void macroTask(void* ignore);

extern pros::Task controlDrive;
extern pros::Task armAsync;

// Structures
struct Vector2 {
  double length;
  int speed;
  int rate;
};

// struct PID {
//   double current, error, last, output, slewOutput;
// };

// It's Everyday Functions Bro
void wait(int ms);
void print(const char * text);
void setReset(bool set);

// Image declaration
LV_IMG_DECLARE(intro);
LV_IMG_DECLARE(logo);

LV_IMG_DECLARE(michael1);
LV_IMG_DECLARE(michael2);

// Motor ports
#define RFPORT 1
#define RBPORT 2
#define LFPORT 9
#define LBPORT 10

#define RACK 5
#define ARM 6

#define FLAPL 8
#define FLAPR 3

// ADI
#define RACKPOT 1
#define ARMLIMIT 2

#define ULTRARPING 3
#define ULTRARECHO 4

#define ULTRALPING 5
#define ULTRALECHO 6

#define ARMPOT 7

#define GYRO 8

// Vision Constants
#define FRONTVISION 11

#define CUBE_PURPLE 1
#define CUBE_ORANGE 2
#define CUBE_GREEN 3
#define BLUE_ZONE 4
#define RED_ZONE 5

// Constants
#define RACK_DOWN 900
#define RACK_UP 3615
#define RACK_TOWER 1800

#define ARM_BOTTOM 0
#define ARM_LOW_TOWER 1.3
#define ARM_LOW_TOWER_MANUAL 1.2
#define ARM_LOW_TOWER_DESCORE 1.1
#define ARM_MID_TOWER 1.7
#define ARM_MID_TOWER_DESCORE 1.7

#define MODE_CHASSIS 1
#define MODE_RACK 2
#define MODE_ARM 3

#define SLOT_RED 1
#define SLOT_BLUE 2
#define SLOT_SKILLS 3

using namespace okapi;

#ifdef __cplusplus
extern "C" {
#endif
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <ostream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdlib>

#include <vector>

#include <stdio.h>
#include <stdarg.h>

#endif

#endif  // _PROS_MAIN_H_
