#include "main.h"

#define ARM_BOTTOM 0
#define ARM_LOW_TOWER 1.25
#define ARM_LOW_TOWER_MANUAL 1.25
#define ARM_LOW_TOWER_DESCORE 1.25
#define ARM_MID_TOWER 1.7
#define ARM_MID_TOWER_DESCORE 1.7

extern pros::Motor ArmMotor;
extern pros::ADIDigitalIn Limit;

class Arm {
  public:
    Arm();
    ~Arm();

    Arm& withTol(double tolerance_ = 0.1);

    Arm& move(double target_, int speed_, int rate_ = 9);
    Arm& tower(int tower);
    Arm& zero();

    void waitUntilSettled();

    void reset();

    // Motor States
    void tarePos();
    void setBrakeType(pros::motor_brake_mode_e_t type_);

    // Task States
    int getMode();
    bool getState();
    bool getMacroState();

    // Sensors
    double * getPos();
    bool * getLimit();

    static void start(void *ignore);
    void run();
    void stop();

    void move(int speed);

  private:
    static bool isRunning;
    static bool isSettled;
    static int mode;
    static int nextCmd;

    static double armPos;
    static bool limitStat;

    static bool macroFin;
    static bool reached;

    static double kP;

    static double tolerance;

    static double target;
    static int speed, rate;

    static double error, output, slewOutput;
};
