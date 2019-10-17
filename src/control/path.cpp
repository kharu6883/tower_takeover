#include "main.h"
#include "control/path.h"

namespace path {
  auto driver = ChassisControllerFactory::create(
    {LFPORT, LBPORT},
    {-RFPORT, -RBPORT},
    AbstractMotor::gearset::green,
    {8_in, 30_in}
  );

  // Acceleration factor
  auto profile = AsyncControllerFactory::motionProfile(
    2, // Max possible velocity in m/s
    1.5, // Max possible acceleration in m/s^2
    6, // Max possible jerk in m/s^3
    driver
  );

  // // Settle factor
  // SettledUtil fin = SettledUtilFactory::create(
  //   100, // Minimum error to be considered settled
  //   0, // Minimum error derivative to be considered settled
  //   10_ms // Minimum time within atTargetError to be considered settled
  // );

  void generate(std::string name, Point pointI) {
    profile.generatePath({
      Point{ 0_ft, 0_ft, 0_deg },
      pointI
    }, name );
  }

  void generate(std::string name, Point pointI, Point pointII) {
    profile.generatePath({
      Point{ 0_ft, 0_ft, 0_deg },
      pointI,
      pointII
    }, name );
  }

  void execute(std::string name, bool reverse) {
    profile.setTarget(name, reverse);
    profile.waitUntilSettled();
  }

  void destroy(std::string name) {
    profile.removePath(name);
  }

  void wait() {
    profile.waitUntilSettled();
  }
}
