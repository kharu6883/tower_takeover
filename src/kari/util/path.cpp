#include "main.h"

#include "kari/autonController.h"
#include "kari/util/path.h"
#include "kari/util/misc.h"

// okapi::ChassisControllerIntegrated Path::driver = ChassisControllerFactory::create(
//   { -14, -15 },
//   { 17, 16 },
//   AbstractMotor::gearset::blue,
//   { 4_in, 10.5_in }
// );
//
// // Acceleration factor
// okapi::AsyncMotionProfileController Path::profile = AsyncControllerFactory::motionProfile(
//   2.5, // Max possible velocity in m/s
//   2, // Max possible acceleration in m/s^2
//   4, // Max possible jerk in m/s^3
//   driver
// );
//
// bool Path::isRunning = false;
//
// std::vector<okapi::Point> Path::waypoints;
//
// void Path::start(void* args) {
//   pros::delay(500);
//   Path* that = static_cast<Path*>(args);
//   that -> update();
// }
//
// void Path::update() {
//   isRunning = true;
//   int last;
//
//   Autonomous Auton;
//
//   while(isRunning) {
//     if(Auton.getType() != last) {
//       for(int i = 0; i < profile.getPaths().size(); i++) {
//         profile.removePath(profile.getPaths()[i]);
//       }
//
//       switch(Auton.getType()) {
//         case SLOT_RED: {
//           withPoint({ 5_ft, 3_ft, 0_deg}).gen("red");
//           break;
//         }
//
//         case SLOT_BLUE: {
//           withPoint({ 3_ft, 3_ft, 0_deg}).gen("blue");
//           break;
//         }
//
//         case SLOT_SKILLS: {
//           break;
//         }
//
//         default: {
//           break;
//         }
//       }
//
//       io::master.rumble(" . .");
//       last = Auton.getType();
//     }
//
//     pros::delay(20);
//   }
// }
//
// void Path::stopThread() {
//   isRunning = false;
// }
//
// Path& Path::withPoint(okapi::Point point_) {
//   waypoints.insert(waypoints.end(), point_);
//   return *this;
// }
//
// void Path::gen(std::string name) {
//   switch(waypoints.size()) {
//     case 1: {
//       profile.generatePath({
//         Point{ 0_ft, 0_ft, 0_deg },
//         waypoints[0],
//       }, name );
//       break;
//     }
//
//     case 2: {
//       profile.generatePath({
//         Point{ 0_ft, 0_ft, 0_deg },
//         waypoints[0],
//         waypoints[1]
//       }, name );
//       break;
//     }
//
//     case 3: {
//       profile.generatePath({
//         Point{ 0_ft, 0_ft, 0_deg },
//         waypoints[0],
//         waypoints[1],
//         waypoints[2]
//       }, name );
//       break;
//     }
//
//     default: {
//       std::cout << "Unable to generate - more than 3 paths, or no path specified." << std::endl;
//       break;
//     }
//   }
//
//   waypoints.clear();
// }
//
// void Path::run(std::string name, bool reverse) {
//   for(int i = 0; i < profile.getPaths().size(); i++) {
//     if(profile.getPaths()[i] == name) break;
//
//     if(i >= profile.getPaths().size()) return;
//   }
//
//   profile.setTarget(name, reverse);
//   profile.waitUntilSettled();
// }
//
// void Path::del(std::string name) {
//   profile.removePath(name);
// }
