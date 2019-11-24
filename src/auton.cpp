#include "main.h"

#include "controller/chassis.h"
#include "controller/rack.h"
#include "controller/arm.h"

#include "controller/misc.h"
#include "controller/vision.h"

static Chassis chassis;
static Rack rack;
static Arm arm;
static Camera CamFront(FRONTVISION);

// Ignore below. Just for testing stuff.
void tester() {
  std::cout << "Testing" << std::endl;
  chassis.drive(1000, 127, 9);
  io::master.rumble(" .");
}

/*===========================================
  PREMADE FUNCTIONS
===========================================*/

/*===========================================
  RED MATCH AUTONOMOUSES
===========================================*/
void red1() {

}

/*===========================================
  BLUE MATCH AUTONOMOUSES
===========================================*/
void blue1() {

}

/*===========================================
  SKILLS AUTONOMOUSES
===========================================*/
void skills1() {

}
