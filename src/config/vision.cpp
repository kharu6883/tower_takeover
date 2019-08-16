#include "main.h"

// Signatures
// pros::vision_signature_s_t CUBE_PURPLE = {1, {255, 255, 255}, 1385, 2559, 1972, 4997, 7325, 6162, 3, 0};
// pros::vision_signature_s_t CUBE_ORANGE = {2, {255, 255, 255}, 7959, 8711, 8334, -2509, -1843, -2176, 3, 0};
// pros::vision_signature_s_t CUBE_GREEN = {3, {255, 255, 255}, -6969, -5859, -6414, -3289, -2055, -2672, 3, 0};
// pros::vision_signature_s_t SIG_4 = {4, {255, 255, 255}, 0, 0, 0, 0, 0, 0, 3, 0};
// pros::vision_signature_s_t SIG_5 = {5, {255, 255, 255}, 0, 0, 0, 0, 0, 0, 3, 0};
// pros::vision_signature_s_t SIG_6 = {6, {255, 255, 255}, 0, 0, 0, 0, 0, 0, 3, 0};
// pros::vision_signature_s_t SIG_7 = {7, {255, 255, 255}, 0, 0, 0, 0, 0, 0, 3, 0};

// Vision Sensor
pros::Vision CamFront(FRONTVISION);

void initVision() {
  CamFront.set_auto_white_balance(false);
}
