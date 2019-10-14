#pragma once
#include <iostream>
#include <map>

struct typeName {
  std::string red;
  std::string blue;
  std::string skills;
};

class Autonomous {
  public:
    Autonomous();

    void runAuton();
    void addAuton(int type_, std::string autonName, void(*function)());

    int getSlot();
    void setSlot(int slot_);
    int getType();
    void setType(int type_);

    int getSize(int type_);
    std::string getName(int type_, int slot_);

  private:
    static bool isInitialized;
    static int type, slot;

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
void r_b_orange();
// Blue
void b_s_8();
void b_b_8();

// Skills
void skills1();
