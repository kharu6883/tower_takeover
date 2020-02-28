#pragma once
#include "api.h"

namespace io {
  extern pros::Controller master;

  extern pros::Motor RollerL, RollerR;

  // Rotates the rollers in a given speed. Use negative values for going down. Uses RPM for speed.
  void roller(int speed);

  void driveRoller(int speed);

  // Rotates the rollers in a given speed to a given point. Use negative values for going down. Uses RPM for speed.
  void roller(double rot, int speed);
}

namespace macro {
  class Slew {
    public:
      Slew(double accel_);
      Slew(double accel_, double decel_);
      Slew(double accel_, double decel_, bool reversible_);

      Slew& withLimit(double input);

      double calculate(double input);

      void setOutput(double output_);
      double getOutput();

      void reset();

    private:
      double accel, decel;
      double input, output, limit;
      bool isReversible, noDecel, isLimited;
  };

  class PID {
    public:
      PID(double kP_);
      PID(double kP_, double kD_);

      PID& withGain(double kP_);
      PID& withGain(double kP_, double kD_);

      double calculate(double target, double input);

      double getError();
      double getOutput();

    private:
      double kP, kD;

      double current, error, last, derivative, output;
  };

  void wait(int ms);
  void print(const char * text);
}
