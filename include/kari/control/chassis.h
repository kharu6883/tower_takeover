#include "main.h"
#include "kari/util/misc.h"

extern pros::Motor LF, LB, RF, RB;
extern pros::ADIGyro Gyro;

#define DRIVING 1
#define TURNING 2
#define STRAFING 3

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
    ~Chassis();

    Chassis& calibrateGyro();

    Chassis& withConst(double kP_ = 0.3, double kD_ = 0.3);
    Chassis& withTol(double tolerance_ = 6);
    Chassis& withSlop(double amp_ = 0.2, double offset_ = 0);
    Chassis& withTarget(double x, double y, int speed, double rate = 4);

    Chassis& drive(); // For withTarget
    Chassis& drive(double target_, int speed_, int rate_ = 4);
    Chassis& turn(double target_, int speed_, int rate_ = 4);
    Chassis& strafe(double target_, int speed_, int rate_ = 4);

    void waitUntilSettled();

    // Chassis Motors
    void tarePos();
    void reset();

    void lock();
    void unlock();

    // Sensors
    int getGyro();

    void setOdom(double * odomL, double * odomR, double * theta_, double * posX_, double * posY_);

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

    static bool usingGyro;

    static double kP, kD;
    static double tolerance, amp, offset;
    static std::vector<ChassisTarget> target;
    static int currentTarget;
    static bool isMultiTarget;

    static double *deltaL, *deltaR, *theta, *posX, *posY;

    static double angle, gyroAmp;

    static double current, last, error, derivative, output, slewOutput;

    static double lastL, lastR, outputL, outputR;

    static double nowTime, lastTime, elapsed;

    double slop(int mode = 0);
};
