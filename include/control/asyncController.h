#include "main.h"

class ControlAsync {
  public:
    ControlAsync();

    static void run(void* args);
    void update();
    void pause();
    void resume();
    void stop();

    // Getters and Setters
    bool isDisabled();

    void reset_drive();

    void drive(double length, int speed, int rate);
    void turn(double length, int speed, int rate);
    void strafe(double length, int speed, int rate);
    void strafe(double length, int speed, int rate, double sturn);

    void drive(double length, int speed, int rate, int pause);
    void turn(double length, int speed, int rate, int pause);
    void strafe(double length, int speed, int rate, int pause);
    void strafe(double length, int speed, int rate, double sturn, int pause);

    void reset_rack();
    void rack(double length, int speed, int rate);

    void reset_arm();
    void arm(double length, int speed, int rate);

  private:
    static bool isRunning;
    static bool isPaused;

    static bool isDrive;
    static bool isTurn;
    static bool isStrafe;
    static bool isRack;
    static bool isArm;

    static bool isWait;
    static int wait;

    static double sturn;

    static Vector2 chassis_target, rack_target, arm_target;

    static PID chassisVar, rackVar, armVar;
};
