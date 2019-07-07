#include "main.h"
using namespace Display;

lv_obj_t * scr;

static lv_style_t overlay;

bool initialized = false;

lv_obj_t * autonStat;
lv_obj_t * btnBack;

int nowScr;

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
    lv_theme_t * th = lv_theme_alien_init(120, NULL);
    lv_theme_set_current(th);

    lv_style_copy(&overlay, &lv_style_plain);
    overlay.body.main_color = LV_COLOR_MAKE(48, 48, 48);
    overlay.body.grad_color = LV_COLOR_MAKE(48, 48, 48);
    overlay.body.border.color = LV_COLOR_GREEN;
    overlay.body.border.width = 2;

    overlay.text.color = LV_COLOR_WHITE;

    lv_obj_t * status = lv_cont_create(lv_layer_top(), NULL);
    lv_obj_set_style(status, &overlay);
    lv_obj_set_pos(status, 2, 2);
    lv_obj_set_size(status, 476, 20);
    lv_cont_set_layout(status, LV_LAYOUT_CENTER);

    autonStat = lv_label_create(status, NULL);
    lv_obj_set_style(autonStat, &overlay);
    lv_obj_set_y(autonStat, 2);

    btnBack = lv_btn_create(lv_layer_top(), NULL);
    setButton(btnBack, 420, SYMBOL_HOME" Home", 100, 40, 5, 190, lv_layer_top());

    print("Display initialized!");
    initialized = true;
  }

  scr = lv_page_create(NULL, NULL);
  lv_scr_load(scr);
}

void BrainDisplay::main() {
  nowScr = 0;

  lv_obj_t * btnAuton = lv_btn_create(scr, NULL);
  setButton(btnAuton, 1, SYMBOL_LIST" Autonomous", 200, 40, 250, 0, NULL);

  lv_obj_t * btnSensor = lv_btn_create(scr, NULL);
  setButton(btnSensor, 2, SYMBOL_GPS" Sensors", 200, 40, 250, 65, NULL);

  lv_obj_t * btnCamera = lv_btn_create(scr, NULL);
  setButton(btnCamera, 3, SYMBOL_IMAGE" Camera", 200, 40, 250, 110, NULL);

  lv_obj_t * btnSetting = lv_btn_create(scr, NULL);
  setButton(btnSetting, 4, SYMBOL_SETTINGS" Settings", 200, 40, 250, 155, NULL);
}

void BrainDisplay::auton() {
  nowScr = 1;

  int size = SlotName.size();

  lv_obj_t * btnAutonm[] = {};
  for(int i = 0; i < size; i++) {
    if(i == 0) {
      btnAutonm[i] = lv_btn_create(scr, NULL);
      setButton(btnAutonm[i], i, getName(i), 250, 40, 200, 40, NULL);
      lv_btn_set_action(btnAutonm[i], LV_BTN_ACTION_CLICK, auton_click_action);
    } else {
      btnAutonm[i] = lv_btn_create(scr, NULL);
      setButton(btnAutonm[i], i, getName(i), 250, 40, 200, i * 45 + 20, NULL);
      lv_btn_set_action(btnAutonm[i], LV_BTN_ACTION_CLICK, auton_click_action);
    }
  }
}

void BrainDisplay::sensor() {
  nowScr = 2;
}

void BrainDisplay::camera() {
  nowScr = 3;
}

void BrainDisplay::setting() {
  nowScr = 4;
}

void BrainDisplay::update(void* ignore) {
  std::string name, now, last;
  const char * c;

  name = "Auton Selected: ";

  while(true) {
    if(nowScr != 0) lv_obj_set_pos(btnBack, 5, 190);
      else lv_obj_set_pos(btnBack, -100, -100);

    // Auton name display
    now = getName(getSlot());
    if(last != now) {
      name.erase(name.begin() + 16, name.end());
      name.append(now);
      c = name.c_str();
    }

    lv_label_set_text(autonStat, c);
    last = now;
    wait(2);
  }
}

// Macros

void BrainDisplay::setLabel(lv_obj_t * labelID, const char * text, lv_coord_t x, lv_coord_t y) {
  lv_label_set_text(labelID, text);
  lv_obj_set_pos(labelID, x, y);
}

void BrainDisplay::setButton(lv_obj_t * buttonID, int uniqueID, const char * name, lv_coord_t w, lv_coord_t h, lv_coord_t x, lv_coord_t y, lv_obj_t * base) {
  lv_obj_set_pos(buttonID, x, y);
  lv_obj_set_size(buttonID, w, h);
  lv_obj_set_free_num(buttonID, uniqueID);
  lv_btn_set_action(buttonID, LV_BTN_ACTION_CLICK, btn_click_action);
  lv_obj_t * buttonLabel = lv_label_create(buttonID, NULL);
  setLabel(buttonLabel, name, 0, 0);
  lv_obj_align(buttonLabel, buttonID, LV_ALIGN_CENTER, 0, 0);
}
