#pragma once
#include "pros/apix.h"

class Display {
  public:
    Display();

    void main();
    void auton();
    void sensor();
    void camera();
    void setting();


    void run();
    static void start(void* ignore);

  private:
    int lastAutonType, lastAutonSlot;

    void remoteUpdate();

    // Macros
    lv_obj_t * createLabel(int x, int y, std::string text, lv_obj_t * parent);
    lv_obj_t * createButton(int id, int x, int y, int width, int height, std::string text, lv_obj_t * parent, lv_action_t action);
    lv_obj_t * drawRectangle(int x, int y, int width, int height, lv_color_t stroke, lv_color_t fill, lv_obj_t * parent);
};
