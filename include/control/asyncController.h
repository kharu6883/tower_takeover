struct Vector2 {
  double length;
  int speed;
  int rate;
};

class ControlAsync {
  public:
    ControlAsync();

    void run(void* args);
    void reset();

    void rackAsync(Vector2 target);
    void armAsync(Vector2 target);

  private:
    bool isDrive;
    bool isTurn;

    bool isRack;
    bool isArm;
};
