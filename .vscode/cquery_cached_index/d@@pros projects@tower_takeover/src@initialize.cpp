#include "main.h"
using namespace Display;

void initialize() {
  initAuton();

  Display::BrainDisplay brain;
  brain.main();

  Display::RemoteDisplay remote;

  pros::Task updateDisplay(brain.update);
  pros::Task updateRemote(remote.update);

  wait(2000);
  brain.cleanup();
  std::cout << "Done!" << std::endl;
}

void disabled() {

}

void competition_initialize() {

}
