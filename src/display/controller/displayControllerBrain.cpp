#include "main.h"

#include "config/motor.h"
#include "config/io.h"
#include "config/vision.h"

#include "control/displayController.h"
#include "control/autonController.h"
#include "control/macro.h"
using namespace Display;

static int screen = 0;

static bool isVision;

static lv_style_t overlay;
static lv_style_t mainScr;
static lv_obj_t * scr;

static lv_obj_t * loading;
static lv_obj_t * loader;
static lv_obj_t * loadText;

static bool initialized = false;
static bool isMain = true;

static lv_obj_t * autonStat;
static lv_obj_t * btnBack;

static lv_obj_t * expVal;

static lv_obj_t * visorCont;

static lv_res_t main_click_action(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);

  lv_obj_del(scr);

  Display::BrainDisplay display;

  if(id == 1) { display.auton(); }
  if(id == 2) { display.sensor(); }
  if(id == 3) { display.camera(); }
  if(id == 4) { display.setting(); }

  // Home Button
  if(id == 420) { display.main(); isVision = false; }

  return LV_RES_OK;
}

static lv_res_t auton_click_action(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);
  setAuton(id);

  return LV_RES_OK;
}

static lv_res_t camera_click_action(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);

  switch(id) {
    case 1: CamFront.set_exposure(CamFront.get_exposure() - 15); break;
    case 2: CamFront.set_exposure(CamFront.get_exposure() + 15); break;
  }

  return LV_RES_OK;
}

static lv_res_t settings_click_action(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);

  switch(id) {
    case 1: setReset(true); break;
  }

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

    btnBack = createButton(420, 5, 190, 100, 40, SYMBOL_HOME" Home", lv_layer_top(), main_click_action);

    print("Display initialized!");
    initialized = true;
  }

  screen = 0;
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
  screen = 0;

  lv_obj_t * mainImg = lv_img_create(scr, NULL);
  lv_obj_set_size(mainImg, 240, 240);
  lv_obj_set_pos(mainImg, 0, -5);
  lv_img_set_src(mainImg, &title);

  lv_obj_t * container_main = lv_cont_create(scr, NULL);
  lv_obj_set_style(container_main, &mainScr);
  lv_cont_set_layout(container_main, LV_LAYOUT_CENTER);
  lv_obj_set_pos(container_main, 250, 55);
  lv_obj_set_size(container_main, 200, 170);

  lv_obj_t * btnAuton = createButton(1, 250, 0, 200, 40, SYMBOL_LIST" Autonomous", container_main, main_click_action);
  lv_obj_t * btnSensor = createButton(2, 250, 65, 200, 40, SYMBOL_GPS" Sensors", container_main, main_click_action);
  lv_obj_t * btnCamera = createButton(3, 250, 110, 200, 40, SYMBOL_IMAGE" Camera", container_main, main_click_action);
  lv_obj_t * btnSetting = createButton(4, 250, 155, 200, 40, SYMBOL_SETTINGS" Settings", container_main, main_click_action);
}

void BrainDisplay::auton() {
  isMain = false;
  screen = 1;

  // This needs to happen because it does not return a proper integer
  // It is a way to explicitly cast the variable type which works
  int size = SlotName.size();

  // Making buttons on autonomouses on a selected order from 0 ~ yeet
  lv_obj_t * btnAutonm[] = {};
  for(int i = 0; i < size; i++) {
    if(i == 0) {
      btnAutonm[i] = createButton(i, 200, 40, 250, 40, getName(i), scr, auton_click_action);
    } else {
      btnAutonm[i] = createButton(i, 200, i * 45 + 20, 250, 40, getName(i), scr, auton_click_action);
    }
  }
}

void BrainDisplay::sensor() {
  isMain = false;
  screen = 2;

  lv_obj_t * placeholder = lv_label_create(scr, NULL);
  lv_label_set_text(placeholder, "Hello, World!");
  lv_obj_t * mainImg = lv_img_create(scr, NULL);
  lv_obj_set_size(mainImg, 240, 240);
  lv_obj_set_pos(mainImg, 0, -5);
  lv_img_set_src(mainImg, &michael1);
}

void BrainDisplay::camera() {
  isMain = false;
  screen = 3;

  visorCont = lv_cont_create(scr, NULL);
  lv_obj_set_pos(visorCont, 134, 22);
  lv_obj_set_size(visorCont, 316, 212);
  isVision = true;

  lv_obj_t * expDec = createButton(1, 0, 25, 40, 40, SYMBOL_LEFT, scr, camera_click_action);
  expVal = createLabel(52, 35, "Exp", scr);
  lv_obj_t * expInc = createButton(2, 85, 25, 40, 40, SYMBOL_RIGHT, scr, camera_click_action);
}

void BrainDisplay::setting() {
  isMain = false;
  screen = 4;

  lv_obj_t * arm_reset = createButton(1, 0, 0, 200, 50, "Calibrate Arm", scr, settings_click_action);
}

void BrainDisplay::update() {
  std::string name, now, last;
  const char * c;

  lv_color_t fill;
  lv_obj_t * visor;
  lv_obj_t * visorInfo;
  lv_obj_t * size;

  const char * sigName;

  name = "Auton Selected: ";

  while(true) {

    switch(screen) {
      case 0: {
        lv_obj_set_pos(btnBack, -100, 190);
        break;
      }

      case 1: {
        lv_obj_set_pos(btnBack, 5, 190);
        break;
      }

      case 2: {
        lv_obj_set_pos(btnBack, 5, 190);
        break;
      }

      case 3: {
        lv_obj_set_pos(btnBack, 5, 190);
        std::string exposure;
        lv_obj_clean(visorCont);
        for(int i = 0; i < CamFront.get_object_count(); i++) {
          pros::vision_object_s_t sig = CamFront.get_by_size(i);
          if(sig.signature == 1) { fill = LV_COLOR_PURPLE; sigName = "Purple Cube"; }
          else if(sig.signature == 2) { fill = LV_COLOR_ORANGE; sigName = "Orange Cube"; }
          else if(sig.signature == 3) { fill = LV_COLOR_GREEN; sigName = "Green Cube"; }
          else { fill = LV_COLOR_WHITE; sigName = "Unknown"; }

          visor = drawRectangle(sig.left_coord, sig.top_coord, sig.width, sig.height, LV_COLOR_WHITE, fill, visorCont);
          visorInfo = createLabel(sig.left_coord, sig.top_coord - 20, sigName, visorCont);
        }

        exposure += std::to_string(CamFront.get_exposure());
        c = exposure.c_str();
        lv_label_set_text(expVal, c);
        break;
      }

      case 4: {
        lv_obj_set_pos(btnBack, 5, 190);
        break;
      }

      default: {
        lv_obj_set_pos(btnBack, 5, 190);
        break;
      }
    }

    // Auton name display
    now = getName(getSlot());
    if(last != now) {
      name.erase(name.begin() + 16, name.end());
      name.append(now);
      c = name.c_str();
      lv_label_set_text(autonStat, c);
    }

    last = now;
    pros::delay(20);
  }
}

void BrainDisplay::run(void* ignore) {
  pros::delay(500);
  BrainDisplay* that = static_cast<BrainDisplay*>(ignore);
  that -> update();
}

// Macros

lv_obj_t * BrainDisplay::createLabel(int x, int y, const char * text, lv_obj_t * parent) {
  lv_obj_t * label = lv_label_create(parent, NULL);
  lv_obj_set_pos(label, x, y);
  lv_label_set_text(label, text);

  return label;
}

lv_obj_t * BrainDisplay::createButton(int id, int x, int y, int width, int height, const char * text, lv_obj_t * parent, lv_action_t action) {
  lv_obj_t * button = lv_btn_create(parent, NULL);
  lv_obj_set_pos(button, x, y);
  lv_obj_set_size(button, width, height);
  lv_obj_set_free_num(button, id);
  lv_btn_set_action(button, LV_BTN_ACTION_CLICK, action);
  lv_obj_t * buttonLabel = createLabel(0, 0, text, button);
  lv_obj_align(buttonLabel, button, LV_ALIGN_CENTER, 0, 0);

  return button;
}

lv_obj_t * BrainDisplay::drawRectangle(int x, int y, int width, int height, lv_color_t stroke, lv_color_t fill, lv_obj_t * parent) {
  lv_obj_t * obj = lv_obj_create(parent, NULL);

  lv_style_t *style1 = (lv_style_t *)malloc( sizeof( lv_style_t ));
  lv_style_copy(style1, &lv_style_plain_color);
  style1 -> body.border.color = stroke;
  style1 -> body.border.width = 1;
  style1 -> body.border.part = LV_BORDER_FULL;

  style1 -> body.main_color = fill;
  style1 -> body.grad_color = fill;

  lv_obj_set_style(obj, style1);
  lv_obj_set_pos(obj, x, y);
  lv_obj_set_size(obj, width, height);

  return obj;
}
