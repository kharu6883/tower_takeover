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
};

class Chassis {
  public:
    Chassis();
    Chassis(double * odomL_, double * odomR_, double * theta_, double * posX_, double * posY_);
    ~Chassis();

    Chassis& calibrateGyro();

    Chassis& withConst(double kP_ = 0.3, double kD_ = 0.3);
    Chassis& withTol(double tolerance_ = 1);
    Chassis& withSlop(double amp_ = 0.2, double offset_ = 0);
    Chassis& withTarget(Vector2 point, int speed_, double rate_ = 4);

    Chassis& drive(); // For withTarget
    Chassis& drive(Vector2 point, int speed_, int rate_ = 4);
    Chassis& turn(double theta_, int speed_, int rate_ = 4);
    Chassis& turn(Vector2 point, int speed_, int rate_ = 4);
    Chassis& strafe(double target_, int speed_, int rate_ = 4);

    void waitUntilSettled();

    // Chassis Motors
    void tarePos();
    void reset();

    void lock();
    void unlock();

    // States
    bool getState();
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

    static double kP, kD;
    static double tolerance, amp, offset;
    static std::vector<ChassisTarget> target;
    static int currentTarget;
    static bool isMultiTarget;
    static bool isTurnToPoint;

    static double thetaRel;

    static double *deltaL, *deltaR, *theta, *posX, *posY;

    static double error, last, output, slewOutput;
    static double errorX, errorY, lastX, lastY, turnError, lastTurn;
    static double driveOutput, turnOutput, driveSlewOutput, turnSlewOutput;

    static double nowTime, lastTime, elapsed;

    double slop(int mode = 0);
};
