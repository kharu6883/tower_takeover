#include "okapi/api.hpp"

namespace path {
  void generate(std::string name, okapi::Point pointI);
  void generate(std::string name, okapi::Point pointI, okapi::Point pointII);
  void execute(std::string name, bool reverse);
  void destroy(std::string name);

  void wait();
}
