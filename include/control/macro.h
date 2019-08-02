extern bool isStacking;

/*--------------------------------
    BASIC MOVEMENT
--------------------------------*/

// Moves the rack in a given speed. Use negative values for going down. Uses RPM for speed.
void rack(int speed);

// Moves the arm speed in a given speed. Use negative values for going down. Uses RPM for speed.
void arm(int speed);

// Rotates the rollers in a given speed. Use negative values for going down. Uses RPM for speed.
void roller(int speed);


/*--------------------------------
    COMPLEX MOVEMENT
--------------------------------*/

// Moves the rack to the target, in a given speed.
void rack(double target, int speed, double rate);

// Moves the arm to the target, in a given speed.
void arm(double target, int speed, double rate);

// Macro for putting a cube in the tower.
void tower(int id);


/*--------------------------------
    PID CALCULATION & CONTROL
--------------------------------*/

// Calculates P term with the following equation. (Target - Sensor) * kP
double pTerm(double target, double sensor, double kP);

// Calculates D term with the following equation. (Now - Last)
double dTerm(double now, double last);

// Returns true if the error value is within the set tolerance.
bool isSettled(double error, double tolerance);


/*--------------------------------
    EXTRA FUNCTIONS
--------------------------------*/

// Basic functions that everybody knows.

void wait(int ms);
void print(const char * text);
