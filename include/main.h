#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

#define PROS_USE_SIMPLE_NAMES

#define PROS_USE_LITERALS

// #define DEBUG

// Nice includes below
#include "api.h"
#include "pros/apix.h"
#include "pros/rtos.h"
#include "okapi/api.hpp"

void macroTask(void* ignore);
extern pros::Task armAsync;

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
#define FLAPL 7
#define FLAPR 3

// ADI
#define ULTRARPING 3
#define ULTRARECHO 4

#define ULTRALPING 5
#define ULTRALECHO 6

#define SELECTOR 7

#define GYRO 8

// Vision Constants
#define FRONTVISION 11

#define CUBE_PURPLE 1
#define CUBE_ORANGE 2
#define CUBE_GREEN 3
#define BLUE_ZONE 4
#define RED_ZONE 5

// Global Constants
#define IDLE 0

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
