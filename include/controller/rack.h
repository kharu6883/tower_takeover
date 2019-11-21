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

    Rack& withTol(double tolerance_ = 3);

    Rack& move(double target_, int speed_, int rate_ = 9);

    void waitUntilSettled();

    void reset();

    void setBrakeType(pros::motor_brake_mode_e_t type_);

    bool getState();
    int getPot();

    void run();
    static void start(void *ignore);
    void stop();

    void move(int speed);

  private:
    static bool isRunning;
    static bool isSettled;
    static bool isActive;

    pros::Motor Motor;
    pros::ADIPotentiometer Pot;

    double kP;

    double tolerance;

    double target;
    int speed, rate;

    static double error, output, slewOutput;

};
