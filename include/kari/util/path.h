#include "okapi/api.hpp"

class Path {
  public:
    static void start(void* args);
    void update();
    void stopThread();

    Path& withPoint(okapi::Point point_);

    void gen(std::string name);
    void run(std::string name, bool reverse);
    void del(std::string name);

  private:
    static bool isRunning;

    static okapi::ChassisControllerIntegrated driver;
    static okapi::AsyncMotionProfileController profile;

    static std::vector<okapi::Point> waypoints;
};
