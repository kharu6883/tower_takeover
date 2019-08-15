#include "main.h"

#include "control/displayController.h"
#include "control/autonController.h"
#include "control/macro.h"
using namespace Display;

lv_obj_t * scr;
lv_obj_t * loading;
lv_obj_t * loader;
lv_obj_t * loadText;

static lv_style_t overlay;
static lv_style_t mainScr;

bool initialized = false;
bool isMain = true;

lv_obj_t * autonStat;
lv_obj_t * btnBack;

static lv_res_t btn_click_action(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);

  lv_obj_del(scr);

  Display::BrainDisplay display;

  if(id == 1) display.auton();
  if(id == 2) display.sensor();
  if(id == 3) display.camera();
  if(id == 4) display.setting();

  // Home Button
  if(id == 420) display.main();

  return LV_RES_OK;
}

static lv_res_t auton_click_action(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);
  setAuton(id);

  return LV_RES_OK;
}

BrainDisplay::BrainDisplay() {
  if(!initialized) {
    // Theme & Style init
    lv_theme_t * th = lv_theme_alien_init(120, NULL);
    lv_theme_set_current(th);

    lv_style_copy(&overlay, &lv_style_plain);
    overlay.body.main_color = LV_COLOR_MAKE(48, 48, 48);
    overlay.body.grad_color = LV_COLOR_MAKE(48, 48, 48);
    overlay.body.border.color = LV_COLOR_GREEN;
    overlay.body.border.width = 2;

    overlay.text.color = LV_COLOR_WHITE;

    lv_style_copy(&mainScr, &lv_style_plain);
    mainScr.body.main_color = LV_COLOR_MAKE(48, 48, 48);
    mainScr.body.grad_color = LV_COLOR_MAKE(48, 48, 48);
    mainScr.body.padding.inner = 3;
    mainScr.body.padding.ver = 1;
    mainScr.body.border.width = 0;

    // Loading Screen
    loading = lv_img_create(lv_layer_sys(), NULL);
    lv_obj_set_size(loading, 480, 240);
    lv_obj_set_pos(loading, 0, 0);
    lv_img_set_src(loading, &intro);

    loader = lv_bar_create(lv_layer_sys(), NULL);
    lv_obj_set_size(loader, 400, 4);
    lv_obj_align(loader, lv_layer_sys(), LV_ALIGN_CENTER, 0, 70);
    lv_bar_set_value_anim(loader, 100, 1500);
    lv_bar_set_value(loader, 1);

    loadText = lv_label_create(lv_layer_sys(), NULL);
    lv_obj_set_style(loadText, &overlay);
    lv_obj_set_size(loadText, 80, 40);
    lv_obj_align(loadText, lv_layer_sys(), LV_ALIGN_CENTER, -25, 100);
    lv_label_set_text(loadText, "Now Loading");

    // Overlay & Screen setup
    lv_obj_t * status = lv_cont_create(lv_layer_top(), NULL);
    lv_obj_set_style(status, &overlay);
    lv_obj_set_pos(status, 2, 2);
    lv_obj_set_size(status, 476, 20);
    lv_cont_set_layout(status, LV_LAYOUT_CENTER);

    autonStat = lv_label_create(status, NULL);
    lv_obj_set_style(autonStat, &overlay);
    lv_obj_set_y(autonStat, 2);

    btnBack = lv_btn_create(lv_layer_top(), NULL);
    setButton(btnBack, 420, SYMBOL_HOME" Home", 100, 40, 5, 190);

    print("Display initialized!");
    initialized = true;
  }

  scr = lv_page_create(NULL, NULL);
  lv_scr_load(scr);
}

void BrainDisplay::cleanup() {
  lv_obj_del(loading);
  lv_obj_del(loader);
  lv_obj_del(loadText);
}

void BrainDisplay::main() {
  isMain = true;

  lv_obj_t * mainImg = lv_img_create(scr, NULL);
  lv_obj_set_size(mainImg, 240, 240);
  lv_obj_set_pos(mainImg, 0, -5);
  lv_img_set_src(mainImg, &title);

  lv_obj_t * container_main = lv_cont_create(scr, NULL);
  lv_obj_set_style(container_main, &mainScr);
  lv_cont_set_layout(container_main, LV_LAYOUT_CENTER);
  lv_obj_set_pos(container_main, 250, 55);
  lv_obj_set_size(container_main, 200, 170);

  lv_obj_t * btnAuton = lv_btn_create(container_main, NULL);
  setButton(btnAuton, 1, SYMBOL_LIST" Autonomous", 200, 40, 250, 0);

  lv_obj_t * btnSensor = lv_btn_create(container_main, NULL);
  setButton(btnSensor, 2, SYMBOL_GPS" Sensors", 200, 40, 250, 65);

  lv_obj_t * btnCamera = lv_btn_create(container_main, NULL);
  setButton(btnCamera, 3, SYMBOL_IMAGE" Camera", 200, 40, 250, 110);

  lv_obj_t * btnSetting = lv_btn_create(container_main, NULL);
  setButton(btnSetting, 4, SYMBOL_SETTINGS" Settings", 200, 40, 250, 155);
}

void BrainDisplay::auton() {
  isMain = false;

  // This needs to happen because it does not return a proper integer
  // It is a way to force change the variable type which works
  int size = SlotName.size();

  // Making buttons on autonomouses on a selected order from 0 to ~lOL
  lv_obj_t * btnAutonm[] = {};
  for(int i = 0; i < size; i++) {
    if(i == 0) {
      btnAutonm[i] = lv_btn_create(scr, NULL);
      setButton(btnAutonm[i], i, getName(i), 250, 40, 200, 40);
      lv_btn_set_action(btnAutonm[i], LV_BTN_ACTION_CLICK, auton_click_action);
    } else {
      btnAutonm[i] = lv_btn_create(scr, NULL);
      setButton(btnAutonm[i], i, getName(i), 250, 40, 200, i * 45 + 20);
      lv_btn_set_action(btnAutonm[i], LV_BTN_ACTION_CLICK, auton_click_action);
    }
  }
}

void BrainDisplay::sensor() {
  isMain = false;

  lv_obj_t * placeholder = lv_label_create(scr, NULL);
  lv_label_set_text(placeholder, "Hello, World!");
}

void BrainDisplay::camera() {
  isMain = false;

  lv_obj_t * placeholder = lv_label_create(scr, NULL);
  lv_label_set_text(placeholder, "Hello, World!");
}

void BrainDisplay::setting() {
  isMain = false;

  lv_obj_t * placeholder = lv_label_create(scr, NULL);
  lv_label_set_text(placeholder, "Hello, World!");
  lv_obj_t * mainImg = lv_img_create(scr, NULL);
  lv_obj_set_size(mainImg, 240, 240);
  lv_obj_set_pos(mainImg, 0, -5);
  lv_img_set_src(mainImg, &michael2);
}

void BrainDisplay::update(void* ignore) {
  std::string name, now, last;
  const char * c;

  name = "Auton Selected: ";

  while(true) {
    // Hides the home button whenever at home
    if(isMain) lv_obj_set_pos(btnBack, -100, 190);
      else lv_obj_set_pos(btnBack, 5, 190);

    // Auton name display
    now = getName(getSlot());
    if(last != now) {
      name.erase(name.begin() + 16, name.end());
      name.append(now);
      c = name.c_str();
      lv_label_set_text(autonStat, c);
    }

    last = now;
    wait(20);
  }
}

// Macros

void BrainDisplay::setLabel(lv_obj_t * labelID, const char * text, lv_coord_t x, lv_coord_t y) {
  lv_label_set_text(labelID, text);
  lv_obj_set_pos(labelID, x, y);
}

void BrainDisplay::setButton(lv_obj_t * buttonID, int uniqueID, const char * name, lv_coord_t w, lv_coord_t h, lv_coord_t x, lv_coord_t y) {
  lv_obj_set_pos(buttonID, x, y);
  lv_obj_set_size(buttonID, w, h);
  lv_obj_set_free_num(buttonID, uniqueID);
  lv_btn_set_action(buttonID, LV_BTN_ACTION_CLICK, btn_click_action);
  lv_obj_t * buttonLabel = lv_label_create(buttonID, NULL);
  setLabel(buttonLabel, name, 0, 0);
  lv_obj_align(buttonLabel, buttonID, LV_ALIGN_CENTER, 0, 0);
}
