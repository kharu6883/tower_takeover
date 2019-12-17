#include "kari/util/misc.h"

class Odometry {
  public:
    static void start(void* ignore);
    void run();
    void stop();

  private:
    static bool isRunning;

    static double deltaL, deltaR;

    static double posX, posY, theta;
};
