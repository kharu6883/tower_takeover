#include "main.h"

lv_obj_t * scr;

bool initialized = false;

static lv_res_t btn_click_action(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);

  lv_obj_del(scr);

  Display display;

  if(id == 1) display.auton();
  if(id == 2) display.sensor();
  if(id == 3) display.camera();
  if(id == 4) display.setting();

  if(id == 420) display.main();

  return LV_RES_OK;
}

static lv_res_t auton_click_action(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);
  setAuton(id);

  return LV_RES_OK;
}

Display::Display() {
  if(!initialized) {
    lv_theme_t * th = lv_theme_alien_init(120, NULL);
    lv_theme_set_current(th);

    print("Display initialized!");
    initialized = true;
  }

  scr = lv_page_create(NULL, NULL);
  lv_scr_load(scr);
}

void Display::main() {
  lv_obj_t * btnAuton = lv_btn_create(scr, NULL);
  setButton(btnAuton, 1, SYMBOL_LIST" Autonomous", 200, 40, 250, 0, NULL);

  lv_obj_t * btnSensor = lv_btn_create(scr, NULL);
  setButton(btnSensor, 2, SYMBOL_GPS" Sensors", 200, 40, 250, 65, NULL);

  lv_obj_t * btnCamera = lv_btn_create(scr, NULL);
  setButton(btnCamera, 3, SYMBOL_IMAGE" Camera", 200, 40, 250, 110, NULL);

  lv_obj_t * btnSetting = lv_btn_create(scr, NULL);
  setButton(btnSetting, 4, SYMBOL_SETTINGS" Settings", 200, 40, 250, 155, NULL);
}

void Display::auton() {
  int size = SlotName.size();

  lv_obj_t * btnBack = lv_btn_create(scr, NULL);
  setButton(btnBack, 420, SYMBOL_HOME" Home", 100, 40, 0, 0, lv_scr_act());

  lv_obj_t * btnAutonm[] = {};
  for(int i = 0; i < size; i++) {
    if(i == 0) {
      btnAutonm[i] = lv_btn_create(scr, NULL);
      setButton(btnAutonm[i], i, getName(i), 250, 40, 200, 0, NULL);
      lv_btn_set_action(btnAutonm[i], LV_BTN_ACTION_CLICK, auton_click_action);
    }

    btnAutonm[i] = lv_btn_create(scr, NULL);
    setButton(btnAutonm[i], i, getName(i), 250, 40, 200, i * 45 + 20, NULL);
    lv_btn_set_action(btnAutonm[i], LV_BTN_ACTION_CLICK, auton_click_action);
  }
}

void Display::sensor() {
  lv_obj_t * btnBack = lv_btn_create(scr, NULL);
  setButton(btnBack, 420, SYMBOL_HOME" Home", 100, 40, 0, 0, lv_scr_act());
}

void Display::camera() {
  lv_obj_t * btnBack = lv_btn_create(scr, NULL);
  setButton(btnBack, 420, SYMBOL_HOME" Home", 100, 40, 0, 0, lv_scr_act());
}

void Display::setting() {
  lv_obj_t * btnBack = lv_btn_create(scr, NULL);
  setButton(btnBack, 420, SYMBOL_HOME" Home", 100, 40, 0, 0, lv_scr_act());
}

// Macros

void Display::setLabel(lv_obj_t * labelID, const char * text, lv_coord_t x, lv_coord_t y) {
  lv_label_set_text(labelID, text);
  lv_obj_set_pos(labelID, x, y);
}

void Display::setButton(lv_obj_t * buttonID, int uniqueID, const char * name, lv_coord_t w, lv_coord_t h, lv_coord_t x, lv_coord_t y, lv_obj_t * base) {
  lv_obj_set_pos(buttonID, x, y);
  lv_obj_set_size(buttonID, w, h);
  lv_obj_set_free_num(buttonID, uniqueID);
  lv_btn_set_action(buttonID, LV_BTN_ACTION_CLICK, btn_click_action);
  lv_obj_t * buttonLabel = lv_label_create(lv_scr_act(), NULL);
  setLabel(buttonLabel, name, 0, 0);
  lv_obj_align(buttonLabel, buttonID, LV_ALIGN_CENTER, 0, 0);
}
