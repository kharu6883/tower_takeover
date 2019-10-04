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

    ControlAsync& withSturn(int sturn);
    ControlAsync& withDelay(int ms);
    ControlAsync& withConst(int mode, double kP_, double kD_);

    void reset_rack();
    void rack(double length, int speed, int rate);

    void reset_arm();
    void disable_arm();
    void arm(double length, int speed, int rate);

    void zero_arm();

  private:
    static bool isRunning;
    static bool isPaused;

    static bool isDrive;
    static bool isTurn;
    static bool isStrafe;
    static bool isRack;
    static bool isArm;
    static bool isZeroing;
    static bool isDown;

    static bool isWait;
    static int wait;

    static int sturn;

    static Vector2 chassis_target, rack_target, arm_target;

    static PID chassisVar, rackVar, armVar;

    static double chassis_kP, chassis_kD;
    static double rack_kP;
    static double arm_kP;
};
