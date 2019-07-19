extern bool isStacking;

/*--------------------------------
    BASIC MOVEMENT
--------------------------------*/

// Rolls the flaps inward (intake the cubes).
void roller(int speed);

//Rolls the flaps depending on the suck variable. If the boolean is set to false, the intake will run backwards, spitting out the cubes.
void roller(int speed, bool suck);


/*--------------------------------
    COMPLEX MOVEMENT
--------------------------------*/

// P loop lift. If a negative target is inserted, the lift will decline.
void lift(double target, int speed);


/*--------------------------------
    PID CALCULATION
--------------------------------*/

// Calculates P term with the following equation. (Target - Sensor) * kP
double pTerm(double target, double sensor, double kP);

// Calculates D term with the following equation. (Now - Last)
double dTerm(double now, double last);

// Etc
void wait(int ms);
void print(const char * text);
