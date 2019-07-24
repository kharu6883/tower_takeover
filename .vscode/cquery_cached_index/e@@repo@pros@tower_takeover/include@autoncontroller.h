#pragma once
#include <iostream>
#include <map>

extern std::map<int, void(*)(void)> Autonomous;
extern std::map<int, const char *> SlotName;
extern std::map<int, const char *>::iterator SlotIter;

void initAuton();
void addAuton(const char * autonName, void(*function)());

// Getters and Setters
void setAuton(int slot);
int getSlot();
const char * getName(int slot);

// Auton Declarations
void tester();
void motionTest();
