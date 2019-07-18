#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

#define PROS_USE_SIMPLE_NAMES

#define PROS_USE_LITERALS

#include "api.h"
#include "pros/apix.h"
#include "pros/rtos.h"
#include "okapi/api.hpp"

#include "autonController.h"

#include "display.h"
#include "movement.h"
#include "odometry.h"
#include "path.h"

#include "macro.h"

// Image declaration
LV_IMG_DECLARE(title);
LV_IMG_DECLARE(intro);

// Motor ports & ADI
#define LFPORT 2
#define LBPORT 10
#define RFPORT 3
#define RBPORT 20

#define LIFT 1

#define FLAPL 11
#define FLAPR 12

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

#include <iostream>
#include <chrono>
#include <cstdlib>

#include <vector>

#include <stdio.h>
#include <stdarg.h>

#endif

#endif  // _PROS_MAIN_H_
