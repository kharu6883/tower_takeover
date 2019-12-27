#include "main.h"
#include "kari/util/misc.h"

extern pros::Motor LF, LB, RF, RB;
extern pros::ADIGyro Gyro;

#define DRIVING 1
#define TURNING 2
#define STRAFING 3

struct Vector2 {
  double x;
  double y;
};

struct ChassisTarget {
  double x;
  double y;
  double theta;
  int speed;
  double rate;
  bool reverse;
};

class Chassis {
  public:
    Chassis();
    Chassis(double * odomL_, double * odomR_, double * theta_, double * posX_, double * posY_);
    ~Chassis();

    Chassis& calibrateGyro();

    // Constant Settings
    Chassis& withConst(double kPd = 0.9, double kDd = 0.3, double kPt = 3.3, double kDt = 0.3);
    Chassis& withTol(double tolerance_ = 1);
    Chassis& withSlop(double amp_ = 0.2, double offset_ = 0);

    // Movement Settings
    Chassis& usingEncoder();
    Chassis& usingGyro();
    Chassis& withPoint(Vector2 point, int speed_, double rate_ = 4, bool reverse_ = false);
    Chassis& withTarget(double target_, double theta_, int speed_, double rate_ = 4, bool reverse_ = false);

    Chassis& drive(); // For withTarget
    Chassis& drive(double target_, int speed_, int rate_ = 4);
    Chassis& drive(Vector2 point, int speed_, int rate_ = 4, bool reverse = false);
    Chassis& turn(double theta_, int speed_, int rate_ = 4);
    Chassis& turn(Vector2 point, int speed_, int rate_ = 4);
    Chassis& strafe(double target_, int speed_, int rate_ = 4);

    void waitUntilSettled();

    // Chassis Motor Settings
    void tarePos();
    void reset();

    void lock();
    void unlock();

    // Process Related
    int getMode();
    void setMode(int mode);

    void clearArr();

    // Task
    static void start(void* ignore);
    void run();
    void stop();

    void left(int speed);
    void right(int speed);

  private:
    static bool isRunning;
    static bool isSettled;
    static int mode;

    static double kP_drive, kD_drive, kP_turn, kD_turn;
    static double tolerance, amp, offset;
    static std::vector<ChassisTarget> target;
    static int currTarget;
    static bool isTurnToPoint;

    static double *deltaL, *deltaR, *theta, *posX, *posY;

    static double thetaRel;
    static double error, last, output, slewOutput;
    static double driveError, driveLast, turnError, turnLast;
    static double driveOutput, turnOutput, driveSlewOutput, turnSlewOutput;
    static double totOutputL, totOutputR;

    double slop(int mode = 0);
};
