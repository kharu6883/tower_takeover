#include "main.h"

lv_obj_t * scr;
lv_obj_t * slider;

void start() {
  scr = lv_page_create(NULL, NULL);
  lv_scr_load(scr);
}

void menu() {
  /* lv_obj_t * label = lv_label_create(scr, NULL);
  setLabel(label, "Boiiii", 0, 0);

  lv_obj_t * button = lv_btn_create(scr, NULL);*/

  slider = lv_slider_create(scr, NULL);
  lv_obj_set_size(slider, lv_obj_get_width(scr) - 50, LV_DPI / 3);
  lv_obj_align(slider, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
  lv_slider_set_value(slider, 30);
}

// Macros for display objects

void setLabel(lv_obj_t * labelId, const char * text, lv_coord_t x, lv_coord_t y) {
  lv_label_set_text(labelId, text);
  lv_obj_set_pos(labelId, x, y);
}
