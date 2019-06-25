#include "main.h"

lv_obj_t * scr;

lv_style_t btnStyle;

static lv_res_t btn_click_action(lv_obj_t * btn) {
  uint8_t id = lv_obj_get_free_num(btn);
  return LV_RES_OK;
}

Display::Display() {
  lv_theme_t * th = lv_theme_alien_init(10, NULL);
  lv_theme_set_current(th);

  main();
}

Display::~Display() {
  std::cout << "Goodbye." << std::endl;
}

void Display::main() {
  scr = lv_page_create(NULL, NULL);
  lv_scr_load(scr);

  lv_obj_t * btnAuton = lv_btn_create(scr, NULL);
  setButton(btnAuton, 1, "Bonjour", LV_ALIGN_CENTER, 250, 50, 0, 0);
}

void Display::auton() {

}

void Display::sensor() {

}

void Display::camera() {

}

void Display::setting() {

}

// Macros

void Display::setLabel(lv_obj_t * labelID, const char * text, lv_coord_t x, lv_coord_t y) {
  lv_label_set_text(labelID, text);
  lv_obj_set_pos(labelID, x, y);
}

void Display::setButton(lv_obj_t * buttonID, int uniqueID, const char * name, lv_align_t align, lv_coord_t w, lv_coord_t h, lv_coord_t x, lv_coord_t y) {
  lv_obj_align(buttonID, scr, align, x, y);
  lv_obj_set_free_num(buttonID, uniqueID);
  lv_btn_set_action(buttonID, LV_BTN_ACTION_CLICK, btn_click_action);
  lv_obj_set_size(buttonID, w, h);
  lv_obj_set_style(buttonID, &btnStyle);
  lv_obj_t * buttonLabel = lv_label_create(scr, NULL);
  setLabel(buttonLabel, name, 0, 0);
  lv_obj_align(buttonLabel, buttonID, LV_ALIGN_CENTER, 0, 0);
}
