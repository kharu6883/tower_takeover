#include "main.h"
#include "config.h"

namespace display {
  void brainDisplay() {
    lv_obj_t * label1 = lv_label_create(lv_scr_act(), NULL);

    lv_label_set_text(label1, "Hello world!");

    lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);
  }

  void remoteDisplay() {

  }
}
