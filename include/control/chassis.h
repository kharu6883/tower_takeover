#include "main.h"

#define LFPORT 9
#define LBPORT 10
#define RFPORT 1
#define RBPORT 2

class Chassis {
  public:
    Chassis(double kP, double kD);
    ~Chassis();

    Chassis& withTolerance(double tolerance_);
    Chassis& withSlop(double amp_ = 0, double offset_ = 0);
    Chassis& runOnThread();

    Chassis& drive(double target_, int speed_, int rate_ = 9);
    Chassis& turn(double target_, int speed_, int rate_ = 9);
    Chassis& strafe(double target_, int speed_, int rate_ = 9);

    Chassis& align(double target_, double tolerance_);

    void waitUntilSettled();

    void reset();

    void run();
    static void start(void* ignore);
    void pause();
    void stop();

  private:
    static Chassis* that;
    static bool isRunning;
    static bool isSettled;
    bool usingThread;

    pros::Motor LF, LB, RF, RB;

    double kP, kD, amp, offset;
    double target;
    int speed, rate;

    double deltaL, deltaR;
    double current, last, error, derivative, output, slewOutput;

    void left(int speed);
    void right(int speed);

    double slop(int mode = 0);
};
