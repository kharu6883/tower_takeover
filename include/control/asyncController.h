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

    void hold_angle();

    ControlAsync& withGyro();
    ControlAsync& withSturn(int sturn);
    ControlAsync& withConst(int mode, double kP_, double kD_);

    void reset_rack();
    void rack(double length, int speed, int rate);

    void reset_arm();
    void arm(double length, int speed, int rate);

    void disable_drive();
    void disable_rack();
    void disable_arm();

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

    static bool usingGyro;

    static int sturn;

    static Vector2 chassis_target, rack_target, arm_target;

    static PIDS chassisVar, rackVar, armVar;

    static double chassis_kP, chassis_kD;
    static double rack_kP;
    static double arm_kP;
};
