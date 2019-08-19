struct Vector2 {
  double length;
  int speed;
  int rate;
};

class ControlAsync {
  public:
    ControlAsync();

    static void run(void* args);
    static void reset();
    static void stop();

    // Getters and Setters
    bool isDisabled();

    void drive(double length, int speed, int rate);
    void turn(double length, int speed, int rate);
    void strafe(double length, int speed, int rate);
    void strafe(double length, int speed, int rate, double sturn);

    void drive(double length, int speed, int rate, int pause);
    void turn(double length, int speed, int rate, int pause);
    void strafe(double length, int speed, int rate, int pause);
    void strafe(double length, int speed, int rate, double sturn, int pause);

  private:
    static bool isRunning;

    static bool isDrive;
    static bool isTurn;
    static bool isStrafe;
    static bool isPause;

    static double sturn;
    static double current, error, last, derivative, output, slewOutput;
    static int pause;

    static Vector2 target;
};
