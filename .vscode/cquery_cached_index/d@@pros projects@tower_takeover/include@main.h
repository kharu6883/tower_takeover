#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

#define PROS_USE_SIMPLE_NAMES

#define PROS_USE_LITERALS

#include "api.h"
#include "pros/apix.h"
#include "pros/rtos.h"
#include "okapi/api.hpp"

#include "display.h"
#include "movement.h"
#include "odometry.h"
#include "path.h"

#include "macro.h"

// Motor ports & ADI
#define LFPORT 10
#define LBPORT 9
#define RFPORT 20
#define RBPORT 19

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

#include <stdio.h>
#include <stdarg.h>

#endif

#endif  // _PROS_MAIN_H_
