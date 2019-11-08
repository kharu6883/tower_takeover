#include "main.h"

#define ARM 6
#define ARMLIMIT 2

#define MOVING 1
#define MACRO 2
#define ZEROING 3

#define ARM_BOTTOM 0
#define ARM_LOW_TOWER 1.3
#define ARM_LOW_TOWER_MANUAL 1.2
#define ARM_LOW_TOWER_DESCORE 1.1
#define ARM_MID_TOWER 1.7
#define ARM_MID_TOWER_DESCORE 1.7

class Arm {
  public:
    Arm(double kP_ = 300);
    ~Arm();

    Arm& withTolerance(double tolerance_ = 6);

    Arm& move(double target_, int speed_, int rate_ = 9);
    Arm& tower(int tower);
    Arm& zero();

    void waitUntilSettled();

    void reset();

    // Getters

    bool getState();
    int getMode();
    bool getLimit();

    void run();
    static void start(void *ignore);
    void stop();

  private:
    static bool isRunning;
    static bool isSettled;
    static int mode;
    static int towerMode;

    pros::Motor Motor;
    pros::ADIDigitalIn Limit;

    double kP, tolerance;

    double target;
    int speed, rate;

    double error, output, slewOutput;

    void arm(int speed);
};
