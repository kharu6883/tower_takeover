#include "main.h"

#include "controller/chassis.h"
#include "controller/rack.h"
#include "controller/arm.h"

#include "controller/misc.h"
#include "controller/vision.h"

static Chassis chassis(0.6, 0.6);
static Rack rack;
static Arm arm;
static Camera CamFront(FRONTVISION);

// Ignore below. Just for testing stuff.
void tester() {
  std::cout << "Testing" << std::endl;
}

/*===========================================
  PREMADE FUNCTIONS
===========================================*/

/*===========================================
  RED MATCH AUTONOMOUSES
===========================================*/

/*===========================================
  BLUE MATCH AUTONOMOUSES
===========================================*/

/*===========================================
  SKILLS AUTONOMOUSES
===========================================*/
