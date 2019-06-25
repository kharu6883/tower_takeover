#pragma once

void initAuton();
void addAuton(int slot, const char * autonName, void(*function)());

// Getters and Setters
void setAuton(int slot);
const char * getName(int slot);

// Auton Declarations
void autonboi();
void motionTest();
