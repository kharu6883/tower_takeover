#include "main.h"

#include "control/drive.h"
#include "control/asyncController.h"
#include "control/macro.h"
#include "control/vision.h"
#include "control/path.h"

static Path path;
static ControlAsync Thread;
static Camera CamFront(FRONTVISION);

// Ignore below. Just for testing stuff.
void tester() {
  std::cout << "Testing" << std::endl;
  Thread.hold_angle();
}
