#include "main.h"
#include "kari/util/misc.h"

extern pros::Motor LF, LB, RF, RB;
extern pros::ADIGyro Gyro;

#define LFPORT 1
#define LBPORT 11
#define RFPORT 10
#define RBPORT 20

#define GYRO 7

#define DRIVING 1
#define TURNING 2
#define STRAFING 3

class Chassis {
  public:
    Chassis();
    ~Chassis();

    Chassis& calibrateGyro();

    Chassis& withConst(double kP_ = 0.3, double kD_ = 0.3);
    Chassis& withTol(double tolerance_ = 6);
    Chassis& withSlop(double amp_ = 0.2, double offset_ = 0);
    Chassis& withGyro(double angle_, double gyroAmp_ = 2);
    Chassis& withTarget(double target_, int speed_, double angle_, double gyroAmp_ = 2, double rate_ = 4);

    Chassis& drive();
    Chassis& drive(double target_, int speed_, int rate_ = 4);
    Chassis& turn(double target_, int speed_, int rate_ = 4);
    Chassis& strafe(double target_, int speed_, int rate_ = 4);

    void waitUntilSettled();

    void tarePos();
    void reset();

    void lock();
    void unlock();

    // Getters & Setters

    int getGyro();

    bool getState();
    int getMode();

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
    static std::vector<macro::ChassisTarget> target;
    static int currentTarget;
    static bool isMultiTarget;

    static double angle, gyroAmp;

    static double deltaL, deltaR;
    static double current, last, error, derivative, output, slewOutput;

    static double lastL, lastR, outputL, outputR;

    static double nowTime, lastTime, elapsed;

    double slop(int mode = 0);
};
