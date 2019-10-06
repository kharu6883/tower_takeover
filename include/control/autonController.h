#pragma once
#include <iostream>
#include <map>

struct typeName {
  const char * red;
  const char * blue;
  const char * skills;
};

class Autonomous {
  public:
    Autonomous();

    void runAuton();
    void addAuton(int type, const char * autonName, void(*function)());

    int getSlot();
    void setSlot(int slot_);
    int getType();
    void setType(int type_);

    int getSize();
    const char * getName(int type, int slot);

  private:
    static bool isInitialized;
    static int slot, type;

    static std::map<int, void(*)()> redAuton;
    static std::map<int, void(*)()> blueAuton;
    static std::map<int, void(*)()> skillsAuton;
    static std::map<int, typeName> slotName;
};

void tester();

/*===========================================
  AUTON DECLARATIONS
===========================================*/

// Red
void r_s_8();
void r_b_8();

// Blue
void b_s_8();
void b_b_8();

// Skills
void skills1();
