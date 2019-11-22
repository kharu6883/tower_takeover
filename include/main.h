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
