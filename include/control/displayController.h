#pragma once
#include "pros/apix.h"

namespace Display {
  class BrainDisplay {
    public:
      BrainDisplay();

      void cleanup();

      void main();
      void auton();
      void sensor();
      void camera();
      void setting();

      void update();
      static void run(void* ignore);

    private:
      // Macros
      lv_obj_t * createLabel(int x, int y, std::string text, lv_obj_t * parent);
      lv_obj_t * createButton(int id, int x, int y, int width, int height, std::string text, lv_obj_t * parent, lv_action_t action);
      lv_obj_t * drawRectangle(int x, int y, int width, int height, lv_color_t stroke, lv_color_t fill, lv_obj_t * parent);
  };

  class RemoteDisplay {
    public:
      RemoteDisplay();

      void setText(const char * text);

      void update();
      static void run(void *ignore);

    private:
      static bool isSetting;
      static const char * text;
  };
}
