#pragma once
#include <vector>
#include <array>
#include "pros/apix.h"

// Image Declaration
LV_IMG_DECLARE(splash);
LV_IMG_DECLARE(michael);

struct info {
  lv_obj_t * labelObj;
  std::string name;
  char type;
  void * data;
  std::string last;
};

class Display {
  public:
    Display();

    void cleanUp();

    static void start(void* ignore);
    void run();
    void stop();

    Display& addInfo(std::string name, char type, void *info);

    void setRemoteText(std::string text_);

  private:
    static bool isRunning, isInitialized;

    static int currScr;

    static std::string setText, lastText;

    static std::vector<info> updateInfo;
    static std::vector<lv_obj_t*> autonm;

    lv_obj_t * tab1;
    lv_obj_t * tab2;
    lv_obj_t * tab3;
    lv_obj_t * tab4;
    lv_obj_t * tab5;

    void tabRed(lv_obj_t * parent);
    void tabBlue(lv_obj_t * parent);
    void tabSkills(lv_obj_t * parent);
    void tabSensor(lv_obj_t * parent);
    void tabSetting(lv_obj_t * parent);

    void remoteUpdate();

    // Macros
    lv_obj_t * createLabel(int x, int y, std::string text, lv_obj_t * parent);
    lv_obj_t * createButton(int id, int x, int y, int width, int height, std::string text, lv_obj_t * parent, lv_action_t action, lv_style_t * btn_pr = nullptr, lv_style_t * btn_rel = nullptr);
};
