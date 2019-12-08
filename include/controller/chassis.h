#include "main.h"
#include "controller/misc.h"

extern pros::Motor LF, LB, RF, RB;
extern pros::ADIUltrasonic LSonic, RSonic;
extern pros::ADIGyro Gyro;

#define LFPORT 14 // 14
#define LBPORT 15 // 15
#define RFPORT 17 // 17
#define RBPORT 16 // 16

#define SONIC_L_PING 3
#define SONIC_L_ECHO 4
#define SONIC_R_PING 5
#define SONIC_R_ECHO 6

#define GYRO 7

#define DRIVING 1
#define TURNING 2
#define ALIGNING 3

class Chassis {
  public:
    Chassis();
    ~Chassis();

    Chassis& calibrateGyro();

    Chassis& withConst(double kP_ = 0.3, double kD = 0.3);
    Chassis& withTol(double tolerance_ = 6);
    Chassis& withSlop(double amp_ = 0.2, double offset_ = 0);
    Chassis& withGyro(double angle_, double gyroAmp_ = 2);
    Chassis& withTarget(double target_, int speed_, double angle_, double gyroAmp_ = 2, double rate_ = 4);

    Chassis& drive();
    Chassis& drive(double target_, int speed_, int rate_ = 4);
    Chassis& turn(double target_, int speed_, int rate_ = 4);

    Chassis& align(double target_);

    void waitUntilSettled();

    // Chassis Motors
    void tarePos();
    void reset();

    void lock();
    void unlock();

    // Sensors
    int getGyro();

    // States
    bool getState();
    int getMode();

    void setMode(int mode);

    void clearArr();

    // Task
    void run();
    static void start(void* ignore);
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
