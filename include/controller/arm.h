#include "main.h"

#define ARM 6
#define ARMLIMIT 2

#define ARM_BOTTOM 0
#define ARM_LOW_TOWER 1.3
#define ARM_LOW_TOWER_MANUAL 1.2
#define ARM_LOW_TOWER_DESCORE 1.1
#define ARM_MID_TOWER 1.7
#define ARM_MID_TOWER_DESCORE 1.7

class Arm {
  public:
    Arm();
    ~Arm();

    Arm& withTol(double tolerance_ = 3);

    Arm& move(double target_, int speed_, int rate_ = 9);
    Arm& tower(int tower);
    Arm& zero();

    void waitUntilSettled();
    void keepPosIfSettled();

    void reset();

    // Motor States
    void tarePos();
    void setBrakeType(pros::motor_brake_mode_e_t type_);

    // Task States
    bool getState();
    int getMode();

    // Sensors
    bool getLimit();

    void run();
    static void start(void *ignore);
    void stop();

    void move(int speed);

  private:
    static bool isRunning;
    static bool isSettled;
    static bool keepPos;
    static int mode;
    static int nextCmd;

    static bool reached;

    pros::Motor Motor;
    pros::ADIDigitalIn Limit;

    static double kP;

    double tolerance;

    double target;
    int speed, rate;

    static double error, output, slewOutput;
};
