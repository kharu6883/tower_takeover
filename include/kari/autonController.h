#pragma once
#include <iostream>
#include <map>

#define SLOT_RED 1
#define SLOT_BLUE 2
#define SLOT_SKILLS 3

struct typeName {
  std::string red;
  std::string blue;
  std::string skills;
};

class Autonomous {
  public:
    Autonomous();

    void runAuton();
    void addAuton(int type_, std::string autonName, std::string autonAbbv, void(*function)());

    int getSlot();
    void setSlot(int slot_);
    int getType();
    void setType(int type_);

    int getSize(int type_);
    std::string getName(int type_, int slot_);
    std::string getAbbv(int type_, int slot_);

    void run();
    static void start(void* ignore);

  private:
    static bool isInitialized;
    static int type, slot;

    static std::map<int, void(*)()> redAuton;
    static std::map<int, void(*)()> blueAuton;
    static std::map<int, void(*)()> skillsAuton;
    static std::map<int, typeName> slotName;
    static std::map<int, typeName> slotAbbv;
};

/*===========================================
  AUTON DECLARATIONS
===========================================*/

void tester();

// Red
void redsmallzone11();
void redsmallzone8();
void redsmallzone7();
void redsmallzone5();
void redbigzone();
void redbigzonescore();


// Blue
void bluesmallzone11();
void bluesmallzone8();
void bluesmallzone7();
void bluesmallzone5();
void bluebigzone();
void bluebigzonescore();

// Skills
void skills1();
void skills2();
void skills3();
