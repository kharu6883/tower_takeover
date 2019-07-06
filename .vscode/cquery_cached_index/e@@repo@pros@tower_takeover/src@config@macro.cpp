#include "main.h"

void wait(int ms) { // Just does pros::delay, so nothing to worry about
  pros::delay(ms);
}

void print(const char * text) {
  std::cout << text << std::endl;
}
