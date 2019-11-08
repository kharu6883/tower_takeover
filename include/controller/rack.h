#include "main.h"

#define RACK 5
#define RACKPOT 1

#define RACK_DOWN 900
#define RACK_TOWER 1950
#define RACK_UP 3615

class Rack {
  public:
    Rack(double kP_ = 100);
    ~Rack();

    Rack& withTolerance(double tolerance_ = 3);

    Rack& move(double target_, int speed_, int rate_ = 9);

    void waitUntilSettled();

    void reset();

    // Getters

    bool getState();
    int getPot();

    void run();
    static void start(void *ignore);
    void stop();

  private:
    static bool isRunning;
    static bool isSettled;
    static bool isActive;

    pros::Motor Motor;
    pros::ADIPotentiometer Pot;

    double kP, tolerance;

    double target;
    int speed, rate;

    double error, output, slewOutput;

    void rack(int speed);
};
