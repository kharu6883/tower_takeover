#include "main.h"

lv_obj_t * scr;

void start() {
  scr = lv_page_create(NULL, NULL);
  lv_scr_load(scr);
}

void menu() {

  lv_obj_t * button = lv_btn_create(scr, NULL);
}

// Macros for display objects

void setLabel(lv_obj_t * labelId, const char * text, lv_coord_t x, lv_coord_t y) {
  lv_label_set_text(labelId, text);
  lv_obj_set_pos(labelId, x, y);
}
