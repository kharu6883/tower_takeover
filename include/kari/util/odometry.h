#include "kari/util/misc.h"

extern pros::ADIEncoder LEncoder, REncoder;

#define ENCODERLTOP 1
#define ENCODERLBOTTOM 2
#define ENCODERRTOP 3
#define ENCODERRBOTTOM 4

class Odometry {
  public:

    // Getters & Setters
    double * getL();
    double * getR();

    double * getTheta();
    double * getX();
    double * getY();

    static void start(void* ignore);
    void run();
    void stop();
    void point(double DesiredX, double DesiredY, double rate, double speed);

  private:
    static bool isRunning;

    static double deltaL, deltaR, lastDeltaL, lastDeltaR;

    static double posX, posY, theta;

    static double amp;

    static double output, DesiredX, DesiredY, Desiredtheta;
};
