#include "main.h"
#include "kari/util/misc.h"

extern pros::Motor LF, LB, RF, RB;
extern pros::ADIUltrasonic Ultrasonic;

#define DRIVING_POINT 1
#define DRIVING_DIST 2
#define DRIVING_ULTRASONIC 3
#define TURNING 4
#define STRAFING 5
#define STRAFING_SMART 6

struct Vector2 {
  double x;
  double y;
};

struct ChassisTarget {
  double x;
  double y;
  double theta;
  double direction;
  int speedDrive;
  int speedTurn;
  double rateDrive;
  double rateTurn;
  bool reverse;
};

class Chassis {
  public:
    Chassis();
    Chassis(int * odomL_, int * odomR_, double * theta_, double * posX_, double * posY_);
    ~Chassis();

    /*
    Sets the gain of the drive.

    @param kP The P Gain.
    @param kI The I Gain.
    @param kD The D Gain.
    @param windUp The integral windup limit.
    */
    Chassis& withGain(double kP = 0.5, double kI = 1.3, double kD = 0.1, double windUp = 170);

    /*
    Sets the gain of the turn.

    @param kP The P Gain.
    @param kI The I Gain.
    @param kD The D Gain.
    @param windUp The integral windup limit.
    */
    Chassis& withTurnGain(double kP = 2, double kI = 0.3, double kD = 3.3, double windUp = 120);

    /*
    Sets the tolerance.

    @param tolerance The tolerance.
    */
    Chassis& withTol(double tolerance_ = 1);

    /*
    Sets the parameters of the slop correction.

    @param offset The offset of the slop - for strafing.
    @param amp The intensity of slop correction.
    */
    Chassis& withSlop(double offset_ = 0, double amp_ = 0.2);

    /*
    Sets the target angle.

    @param theta The target angle in degrees.
    @param speed The speed in which the robot will turn.
    @param rate The rate in which the slew will accelerate.
    */
    Chassis& withAngle(double theta_, int speed_, double rate_ = 4);

    /*
    Sets the desired waypoints to drive through.

    @param point The X and Y coordinates of the desired point.

    For example, {500, 600}, {1000, 2000}
    */
    Chassis& withPoints(Vector2 point, ...);
    Chassis& withSettings(int driveSpeed_, int turnSpeed, double driveRate = 4, double turnRate_ = 4, bool reverse_ = false);
    Chassis& withTarget(double target_, double theta_, int speed_, double rate_ = 4, bool reverse_ = false);

    // Actuators
    Chassis& drive();
    Chassis& drive(Vector2 point, int speed_, int rate_ = 4, bool reverse_ = false);
    Chassis& drive(double target_, int speed_, double rate_ = 4);
    Chassis& driveUltrasonic(double target_, int speed_, int rate_ = 4);
    Chassis& turn(Vector2 point, int speed_, int rate_ = 4);
    Chassis& turn(double theta_, int speed_, int rate_ = 4);
    Chassis& strafe(double target_, int speed_, int rate_ = 4);
    Chassis& smartstrafe(double direction_, double theta_, double drivespeed_ = 80, double turnspeed_ = 50, double rate_ = 4, double rate2_ = 4);

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

    static double kP_drive, kI_drive, kD_drive, kP_turn, kI_turn, kD_turn, kI_Windup;
    static double tolerance, amp, offset;
    static std::vector<Vector2> waypoints;
    static std::vector<ChassisTarget> target;
    static int currTarget;
    static bool isUsingPoint, isUsingAngle;

    static int *odomL, *odomR;
    static double *theta, *posX, *posY;

    static double current, initL, initR, deltaL, deltaR;
    static double driveError, driveIntegral, driveLast, turnError, turnIntegral, turnLast;
    static double driveOutput, driveOutput2, driveOutput3, driveOutput4, turnOutput, driveSlewOutput, driveSlewOutput2, driveSlewOutput3, driveSlewOutput4, turnSlewOutput;
    static double totOutputL, totOutputR;

    double slop(int mode = 0);
};
