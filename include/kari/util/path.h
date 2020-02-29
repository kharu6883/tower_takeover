#include "main.h"
#include "kari/util/misc.h"

class Path {
  public:
    Path();

    Path& withLookAheadDistance();

  private:
    static bool isRunning;

    static okapi::ChassisControllerIntegrated driver;
    static okapi::AsyncMotionProfileController profile;

    static std::vector<okapi::Point> waypoints;
};
