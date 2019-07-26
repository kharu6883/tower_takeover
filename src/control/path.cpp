#include "main.h"

namespace path {
  auto driver = ChassisControllerFactory::create(
    {LFPORT, LBPORT},
    {-RFPORT, -RBPORT},
    AbstractMotor::gearset::green,
    {4.125_in, 30_in}
  );

  // Acceleration factor
  auto profile = AsyncControllerFactory::motionProfile(
    1, // Max possible velocity in m/s
    3, // Max possible acceleration in m/s^2
    10, // Max possible jerk in m/s^3
    driver
  );

  // Settle factor
  SettledUtil fin = SettledUtilFactory::create(
    100, // Minimum error to be considered settled
    0, // Minimum error derivative to be considered settled
    10_ms // Minimum time within atTargetError to be considered settled
  );

  void generate(std::string name, okapi::QLength x, okapi::QLength y, okapi::QAngle angle) {
    profile.generatePath({
      Point{ 0_ft, 0_ft, 0_deg },
      Point{ x, y, angle }
    }, name );
  }

  void execute(std::string name, bool reverse) {
    profile.setTarget(name, reverse);
  }

  void destroy(std::string name) {
    wait();
    profile.removePath(name);
  }

  void wait() {
    profile.waitUntilSettled();
  }
}
