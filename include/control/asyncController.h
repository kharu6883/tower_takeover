struct Vector2 {
  double length;
  int speed;
  int rate;
};

class ControlAsync {
  public:
    ControlAsync();

    static void run(void* args);
    void reset();
    void stop();

    // Getters and Setters
    bool isDisabled();

    void driveAsync(double length, int speed, int rate);
    void turnAsync(double length, int speed, int rate);

  private:
    bool isDrive;
    bool isTurn;

    Vector2 target;
};
