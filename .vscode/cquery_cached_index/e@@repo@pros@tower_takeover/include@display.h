#pragma once
#include "pros/apix.h"

class Display {
  public:
    Display();
    ~Display();

    void start();    
    void main();
    void auton();
    void sensor();
    void camera();
    void setting();

  private:
    void setLabel(lv_obj_t * labelId, const char * text, lv_coord_t x, lv_coord_t y);
};
