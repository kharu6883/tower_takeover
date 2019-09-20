struct Vector2 {
  double length;
  int speed;
  int rate;
};

struct PID {
  double current, error, last, output, slewOutput;
};

class ControlAsync {
  public:
    ControlAsync();

    static void run(void* args);
    void update();
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

    static bool isDrive;
    static bool isTurn;
    static bool isStrafe;
    static bool isRack;
    static bool isArm;

    static bool isPause;
    static int pause;

    static double sturn;

    static Vector2 chassis_target, rack_target, arm_target;

    static PID chassisVar, rackVar, armVar;
};
