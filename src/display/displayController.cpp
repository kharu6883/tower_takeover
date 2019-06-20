#include "main.h"

lv_obj_t * scr;

void start() {
  lv_obj_t * scr = lv_page_create(NULL, NULL);
  lv_scr_load(scr);
}

void brainDisplay() {
  lv_obj_t * label1 = lv_label_create(scr, NULL);
  lv_label_set_text(label1, "Hello world!");
  lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);
}

void remoteDisplay() {

}
