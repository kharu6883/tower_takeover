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

// Moves the lift. This is for autonomouses only.
void liftAsync(double target, int speed, double rate);

// Opens or closes the claw to an absolute target, in a given speed.
void claw(double target, int speed);

// Moves the claw. When boolean is set to true, it will open. If it is set to false, it will close.
void clawTo(bool open);


/*--------------------------------
    PID CALCULATION
--------------------------------*/

// Calculates P term with the following equation. (Target - Sensor) * kP
double pTerm(double target, double sensor, double kP);

// Calculates D term with the following equation. (Now - Last)
double dTerm(double now, double last);

// Provides a slew output based on the input.
double slew(double target, double actual, double rate);

/*--------------------------------
    EXTRA FUNCTIONS
--------------------------------*/

// Basic functions that everybody knows.

void wait(int ms);
void print(const char * text);
