#include "api.h"

extern bool isStacking;

/*--------------------------------
    BASIC MOVEMENT
--------------------------------*/

// Moves the rack in a given speed. Use negative values for going down. Uses RPM for speed.
void rack(int speed);

// Moves the arm speed in a given speed. Use negative values for going down. Uses RPM for speed.
void arm(int speed);

// Rotates the rollers in a given speed. Use negative values for going down. Uses RPM for speed.
int roller(int speed);

// Rotates the rollers in a given speed to a given point. Use negative values for going down. Uses RPM for speed.
int roller(double rot, int speed);


/*--------------------------------
    COMPLEX MOVEMENT
--------------------------------*/

// Moves the rack to the target, in a given speed.
void rack(double target, int speed, double rate);

// Moves the arm to the target, in a given speed.
void arm(double target, int speed, double rate);

// Macro for towers. 1 - Low Tower, 2 - Mid Tower
void tower(int tower);

// Resets the arm using the limit switch.
void armReset();


/*--------------------------------
    VISION FEED
--------------------------------*/

// Gets information from the selected signature.
pros::vision_object_s_t getVisionSig(int id, int size);



/*--------------------------------
    CONTROL & PID CALCULATION
--------------------------------*/

// Calculates P term with the following equation. (Target - Sensor) * kP
double pTerm(double target, double sensor, double kP);

// Calculates D term with the following equation. (Now - Last)
double dTerm(double now, double last);

// Returns true if the error value is within the set tolerance.
bool isSettled(double error, double tolerance);

// Calculates slop, utilizing all four motors.
double slop();

// Calculates slop. If set to 1, will return turning slop calculation. If set to 2, strafe calculation.
double slop(int mode, double offset);

// Note - functions like 'wait' and 'print' have been moved to main.h
