#pragma once
#include "pros/apix.h"

class Display {
  public:
    Display();
    //~Display();

    void start();
    void main();
    void auton();
    void sensor();
    void camera();
    void setting();

  private:
    // Macros
    void setLabel(lv_obj_t * labelId, const char * text, lv_coord_t x, lv_coord_t y);
    void setButton(lv_obj_t * buttonID, int uniqueID, const char * name, lv_coord_t w, lv_coord_t h, lv_coord_t x, lv_coord_t y, lv_obj_t * base);
};
