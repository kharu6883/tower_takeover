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

    void driveAsync(double length, int speed, int rate);
    void turnAsync(double length, int speed, int rate);

  private:
    static bool isDrive;
    static bool isTurn;

    static Vector2 target;
};
