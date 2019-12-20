#include "kari/util/misc.h"

extern pros::ADIEncoder LEncoder, REncoder;

class Odometry {
  public:

    // Getters & Setters
    double * getL();
    double * getR();

    double * getThetaRad();
    double * getThetaDeg();
    double * getX();
    double * getY();

    static void start(void* ignore);
    void run();
    void stop();
    void point(double DesiredX, double DesiredY, double rate, double speed);

  private:
    static bool isRunning;

    static double deltaL, deltaR, lastDeltaL, lastDeltaR;

    static double thetaRad, thetaDeg, posX, posY;

    static double amp;

    static double output, DesiredX, DesiredY, Desiredtheta;
};
