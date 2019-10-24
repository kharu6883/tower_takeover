#include "main.h"

class Chassis {
  public:
    Chassis(double kP, double kD);

    void move(int speed);
    void move(double length, int speed, double rate);

    void reset();

    Chassis& withSlew(int accel, int decel);
    Chassis& withSlop(double amp, double offset);
    Chassis& isThread();

  private:
    static bool isRunning;

    double kP, kD;

    double deltaL, deltaR;
    double current, last, error, derivative, output;
    double slewOutput;

    bool usingSlew, usingThread;
};
