#include "okapi/api.hpp"

namespace path {
  void generate(std::string name, okapi::QLength x, okapi::QLength y, okapi::QAngle angle);
  void execute(std::string name, bool reverse);
  void destroy(std::string name);

  void wait();
}
