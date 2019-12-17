#include "main.h"

#include "kari/util/vision.h"
#include "kari/control/chassis.h"

using namespace pros;

Camera::Camera(int cameraID_) {
  cameraID = cameraID_;
}

Camera& Camera::withSig(int sig_) {
  sig = sig_;
  return *this;
}

// Camera& Camera::withDim(int width_, int height_) {
//   width = width_;
//   height = height_;
//   return *this;
// }

Camera& Camera::withArea(int low_, int high_) {
  low = low_;
  high = high_;
  return *this;
}

std::map<int, vision_object_s_t> Camera::getFeed() {
  Vision reader(cameraID);
  std::map<int, vision_object_s_t> objects;

  for(int i = 0; i < reader.get_object_count(); i++) {
    vision_object_s_t rtn = reader.get_by_size(i);
    if(sig != 0 && rtn.signature != sig) continue;

    if(low != 0 || high != 0) {
      if(!(rtn.width * rtn.height >= low && rtn.width * rtn.height <= high)) continue;
    }
    objects.insert(std::make_pair(objects.size(), rtn));
  }

  std::cout << sizeof(reader) << std::endl;

  return objects;
}

void Camera::target(int sig, int size, int low, int high, double tolerance) {

  // const double kP = 0.6;
  //
  // vision_object_s_t rtn;
  // double output;
  // Chassis chassis;
  //
  // while(true) {
  //   rtn = withSig(sig).withArea(low, high).getFeed()[size];
  //   output = (150 - rtn.x_middle_coord) * kP;
  //
  //   chassis.left(output);
  //   chassis.right(-output);
  //
  //   if(-tolerance < output < tolerance) break;
  //   pros::delay(20);
  // }
  //
  // chassis.left(0);
  // chassis.right(0);
}
