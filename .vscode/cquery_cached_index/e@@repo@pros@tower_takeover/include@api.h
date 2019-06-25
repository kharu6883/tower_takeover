#ifndef _PROS_API_H_
#define _PROS_API_H_

#ifdef __cplusplus
#include <cerrno>
#include <cmath>
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#else /* (not) __cplusplus */
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#endif /* __cplusplus */

#define PROS_VERSION_MAJOR 3
#define PROS_VERSION_MINOR 1
#define PROS_VERSION_PATCH 6
#define PROS_VERSION_STRING "3.1.6"

#define PROS_ERR (INT32_MAX)
#define PROS_ERR_F (INFINITY)

#include "pros/adi.h"
#include "pros/colors.h"
#include "pros/llemu.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.h"
#include "pros/vision.h"

#ifdef __cplusplus
#include "pros/adi.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"
#include "pros/vision.hpp"
#endif

#endif  // _PROS_API_H_
