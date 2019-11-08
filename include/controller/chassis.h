#include "main.h"

#define LFPORT 9
#define LBPORT 10
#define RFPORT 1
#define RBPORT 2

#define DRIVING 1
#define TURNING 2
#define STRAFING 3
#define ALIGNING 4

class Chassis {
  public:
    Chassis(double kP_ = 0.6, double kD_ = 0.6);
    ~Chassis();

    Chassis& withTolerance(double tolerance_ = 8);
    Chassis& withSlop(double amp_ = 8, double offset_ = 0);

    Chassis& drive(double target_, int speed_, int rate_ = 9);
    Chassis& turn(double target_, int speed_, int rate_ = 9);
    Chassis& strafe(double target_, int speed_, int rate_ = 9);

    Chassis& align(double target_);

    void waitUntilSettled();

    void reset();

    void lock();
    void unlock();

    // Getters

    bool getState();
    int getMode();

    void run();
    static void start(void* ignore);
    void stop();

  private:
    static bool isRunning;
    static bool isSettled;
    static int mode;

    pros::Motor LF, LB, RF, RB;

    double kP, kD;
    double tolerance, amp, offset;
    double target;
    int speed, rate;

    double deltaL, deltaR;
    double current, last, error, derivative, output, slewOutput;

    void left(int speed);
    void right(int speed);

    double slop(int mode = 0);
};
