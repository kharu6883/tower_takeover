#pragma once
#include <iostream>
#include <map>

class Autonomous {
  public:
    Autonomous();

    // Runs the selected auton.
    void runAuton();
    void addAuton(const char * autonName, void(*function)());

    // Gets the current slot selected.
    int getSlot();

    // Sets an auton slot.
    void setSlot(int slot);

    // Gets the size of the SlotName array.
    int getSize();

    // Gets the name of a slot.
    const char * getName(int slot);

  private:
    static bool isInitialized;
    static int autonSlot;

    static std::map<int, void(*)(void)> AutonArray;
    static std::map<int, const char *> SlotName;
};

// Auton Declarations
void tester();

// Slot 1
void r_s_8();
void skills1();
void skills2();


void motionTest();
