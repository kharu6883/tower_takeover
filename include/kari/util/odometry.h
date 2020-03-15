#include "kari/util/misc.h"

extern pros::ADIEncoder LEncoder, REncoder;

class Odom {
  public:

    // Getters & Setters
    int * getL();
    int * getR();
    int * getDL();
    int * getDR();

    double * getThetaDeg();
    double * getThetaRad();
    double * getX();
    double * getY();

    Odom& calibrateGyro();
    Odom& zero();
    Odom& reset();

    static void start(void* ignore);
    void run();
    void stop();

  private:
    static bool isRunning;

    static int currentL, currentR;
    static int deltaL, deltaR, lastDeltaL, lastDeltaR;

    static double inertL, inertR, inertT;
    static double thetaRad, thetaDeg, offset, posX, posY;

    static double output, DesiredX, DesiredY, Desiredtheta;
};
