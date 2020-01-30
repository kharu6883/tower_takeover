#pragma once
#include <vector>
#include <array>
#include "pros/apix.h"

// Image Declaration
LV_IMG_DECLARE(intro);
LV_IMG_DECLARE(logo);

LV_IMG_DECLARE(michael1);
LV_IMG_DECLARE(michael2);

struct info {
  lv_obj_t * labelObj;
  std::string text;
  void * data;
  double last;
};

class Display {
  public:
    Display();

    void main();
    void auton();
    void sensor();
    void camera();
    void setting();

    void tab_main();
    void tab_auton();
    void tab_sensor();
    void tab_setting();

    void run();
    static void start(void* ignore);

    Display& addInfo(std::string text, void *info);

    void setRemoteText(std::string text_);

  private:
    static std::string setText, lastText;

    static std::vector<info> updateInfo;

    void remoteUpdate();

    // Macros
    lv_obj_t * createLabel(int x, int y, std::string text, lv_obj_t * parent);
    lv_obj_t * createButton(int id, int x, int y, int width, int height, std::string text, lv_obj_t * parent, lv_action_t action);
};
