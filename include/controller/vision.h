#include "api.h"
#include <map>
using namespace pros;

class Camera {
  public:
    Camera(int cameraID_);

    // Feed Settings
    Camera& withSig(int sig_);
    // Camera& withDim(int width_, int height_);
    Camera& withArea(int low_, int high_);

    // Return Feed
    std::map<int, vision_object_s_t> getFeed();

    // Drivers

    // Targets the robot to a given signature.
    void target(int sig, int size, int low, int high, double tolerance);

  private:
    int cameraID;
    int sig = 0, width = 0, height = 0, low = 0, high = 0;
};
