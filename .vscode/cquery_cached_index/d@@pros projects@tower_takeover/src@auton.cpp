#include "main.h"
using namespace path;

void autonboi() {
  std::cout << "Hello world!" << std::endl;
}

void motionTest() {
  std::cout << "Reverse Traverse" << std::endl;
  generate("1", 5.5_ft, -1.5_ft, 0_deg);
  execute("1", false);
  destroy("1");

  generate("2", 5_ft, 4.5_ft, 90_deg);
  execute("2", false);
  destroy("2");

  generate("3", 2_ft, 2_ft, 100_deg);
  execute("3", false);
  destroy("3");

  generate("4", 4.5_ft, 4_ft, 0_deg);
  execute("4", false);
  destroy("4");

  generate("5", 5_ft, -4_ft, -10_deg);
  execute("5", false);
  destroy("5");

  generate("5", 9_ft, 0.3_ft, 0_deg);
  execute("5", true);
  destroy("5");

  generate("6", 5_ft, 3_ft, 0_deg);
  execute("6", false);
  destroy("6");

  generate("6", 4_ft, 1_ft, 0_deg);
  execute("6", false);
  destroy("6");

  generate("7", 0_ft, 0_ft, 90_deg);
  execute("7", false);
  destroy("7");
}
