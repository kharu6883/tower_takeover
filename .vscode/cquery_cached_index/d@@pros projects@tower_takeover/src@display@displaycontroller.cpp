#include "main.h"

lv_obj_t * scr;

Display::Display() {
  scr = lv_page_create(NULL, NULL);
  lv_scr_load(scr);

  while(true) {
    lv_task_handler();
    wait(25);
  }
}

Display::~Display() {
  std::cout << "Goodbye." << std::endl;
}

void Display::main() {
  lv_obj_t * button = lv_btn_create(scr, NULL);
}

// Macros
void Display::setLabel(lv_obj_t * labelId, const char * text, lv_coord_t x, lv_coord_t y) {
  lv_label_set_text(labelId, text);
  lv_obj_set_pos(labelId, x, y);
}
