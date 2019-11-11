#include "main.h"

#define LFPORT 9
#define LBPORT 10
#define RFPORT 1
#define RBPORT 2

#define SONIC_L_PING 5
#define SONIC_L_ECHO 6
#define SONIC_R_PING 3
#define SONIC_R_ECHO 4

#define GYRO 8

#define DRIVING 1
#define TURNING 2
#define STRAFING 3
#define ALIGNING 4

class Chassis {
  public:
    Chassis(double kP_ = 0.6, double kD_ = 0.6);
    ~Chassis();

    Chassis& calibrateGyro();

    Chassis& withTolerance(double tolerance_ = 9);
    Chassis& withSlop(double amp_ = 8, double offset_ = 0);

    // Actuators

    Chassis& drive(double target_, int speed_, int rate_ = 9);
    Chassis& turn(double target_, int speed_, int rate_ = 9);
    Chassis& strafe(double target_, int speed_, int rate_ = 9);
    Chassis& arc(double target_, double angle_, int speed_, int rate_ = 9, double gyroAmp_ = 1);

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
    pros::ADIUltrasonic LSonic, RSonic;
    pros::ADIGyro Gyro;

    double kP, kD;
    double tolerance, amp, offset;
    double target;
    int speed, rate;

    double angle, gyroAmp;

    double deltaL, deltaR;
    double current, last, error, derivative, output, slewOutput;

    void left(int speed);
    void right(int speed);

    double slop(int mode = 0);
};
