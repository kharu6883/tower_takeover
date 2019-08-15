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

  private:
    static bool isDrive;
    static bool isTurn;
    static bool isStrafe;

    static double sturn;
    static double current, error, last, derivative, output, slewOutput;

    static Vector2 target;
};
