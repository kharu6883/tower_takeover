#include "kari/util/misc.h"

extern pros::ADIEncoder LEncoder, REncoder;

class Odom {
  public:

    // Getters & Setters
    double * getL();
    double * getR();
    double * getDL();
    double * getDR();

    double * getThetaRad();
    double * getThetaDeg();
    double * getX();
    double * getY();

    void reset(int theta_);

    static void start(void* ignore);
    void run();
    void stop();

  private:
    static bool isRunning;

    static double currentL, currentR;
    static double deltaL, deltaR, lastDeltaL, lastDeltaR;

    static double thetaRad, thetaDeg, posX, posY;

    static double output, DesiredX, DesiredY, Desiredtheta;
};
