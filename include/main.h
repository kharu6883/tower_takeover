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

#include "control/movement.h"
#include "control/macro.h"
#include "control/path.h"

// Image declaration
LV_IMG_DECLARE(title);
LV_IMG_DECLARE(intro);

// Motor ports & ADI
#define LFPORT 9
#define LBPORT 10
#define RFPORT 3
#define RBPORT 4

#define RACK 11
#define ARM 12

#define FLAPL 1
#define FLAPR 2

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
