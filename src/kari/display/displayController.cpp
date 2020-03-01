#include "main.h"

#include "kari/displayController.h"
#include "kari/autonController.h"

#include "kari/util/odometry.h"
#include "kari/util/misc.h"

#include "kari/control/chassis.h"
#include "kari/control/arm.h"

static Autonomous Auton;

static Odom odom;
static Arm arm;

bool Display::isRunning = false,
Display::isInitialized = false;

int Display::currScr = 2;

std::string Display::setText = "",
Display::lastText = "";

std::vector<info> Display::updateInfo;
std::vector<lv_obj_t*> Display::autonm;

static int auton_type = 1;

static lv_style_t style_red;
static lv_style_t style_blue;
static lv_style_t style_skills;

static lv_style_t style_red_released;
static lv_style_t style_blue_released;
static lv_style_t style_skills_released;

static lv_obj_t * scr;
static lv_obj_t * tv;

static lv_obj_t * autonName;

static lv_res_t btn_click_action(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);

  switch(id) {
    case 1: odom.zero(); break;
    case 2: odom.reset(); break;
    case 3: arm.tarePos(); break;

    default: break;
  }

  return LV_RES_OK;
}

static lv_res_t red_click_action(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);

  Auton.setType(1);
  Auton.setSlot(id);

  return LV_RES_OK;
}

static lv_res_t blue_click_action(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);

  Auton.setType(2);
  Auton.setSlot(id);

  return LV_RES_OK;
}

static lv_res_t skills_click_action(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);

  Auton.setType(3);
  Auton.setSlot(id);

  return LV_RES_OK;
}

Display::Display() {
  if(!isInitialized) {
    // Theme & Style init
    lv_theme_t * th = lv_theme_material_init(120, NULL);

    th->bg->body.main_color = LV_COLOR_HEX(0x4a4a4a);
    th->bg->body.grad_color = LV_COLOR_HEX(0x4a4a4a);

    th->tabview.btn.bg->body.main_color = LV_COLOR_HEX(0x4a4a4a);
    th->tabview.btn.bg->body.grad_color = LV_COLOR_HEX(0x4a4a4a);
    th->tabview.btn.bg->body.shadow.width = 0;

    th->tabview.btn.bg->text.color = LV_COLOR_WHITE;
    // th->tabview.btn.rel->text.color = LV_COLOR_WHITE;
    // th->tabview.btn.tgl_rel->text.color = LV_COLOR_WHITE;

    lv_theme_set_current(th);

    lv_style_plain.body.radius = 1;

    // Splash screen
    lv_obj_t * splashImg = lv_img_create(lv_layer_sys(), nullptr);
    lv_img_set_src(splashImg, &splash);
    lv_obj_set_pos(splashImg, 0, -15);

    static lv_style_t preloadStyle;
    lv_style_copy(&preloadStyle, &lv_style_plain);
    preloadStyle.line.width = 3;
    preloadStyle.line.color = LV_COLOR_LIME;

    preloadStyle.body.border.color = LV_COLOR_TRANSP;
    preloadStyle.body.padding.hor = 0;

    lv_obj_t * preload = lv_preload_create(lv_layer_sys(), NULL);
    lv_obj_set_size(preload, 40, 40);
    lv_obj_align(preload, NULL, LV_ALIGN_CENTER, 0, 80);
    lv_preload_set_style(preload, LV_PRELOAD_STYLE_MAIN, &preloadStyle);

    // Overlay
    lv_obj_t * overlay = lv_cont_create(lv_layer_top(), NULL);
    lv_obj_set_size(overlay, 480, 35);
    lv_obj_set_pos(overlay, 0, 215);

    autonName = createLabel(0, 0, "Selected Autonomous: ", overlay);
    lv_obj_align(autonName, NULL, LV_ALIGN_IN_LEFT_MID, 5, -4);

    // Auton Btn Style
    lv_style_copy(&style_red, &lv_style_plain);
    style_red.body.main_color = LV_COLOR_RED;
    style_red.body.grad_color = LV_COLOR_RED;
    style_red.body.border.color = LV_COLOR_WHITE;
    style_red.body.border.width = 2;
    style_red.text.color = LV_COLOR_WHITE;

    lv_style_copy(&style_blue, &lv_style_plain);
    style_blue.body.main_color = LV_COLOR_BLUE;
    style_blue.body.grad_color = LV_COLOR_BLUE;
    style_blue.body.border.color = LV_COLOR_WHITE;
    style_blue.body.border.width = 2;
    style_blue.text.color = LV_COLOR_WHITE;

    lv_style_copy(&style_skills, &lv_style_plain);
    style_skills.body.main_color = LV_COLOR_GRAY;
    style_skills.body.grad_color = LV_COLOR_GRAY;
    style_skills.body.border.color = LV_COLOR_WHITE;
    style_skills.body.border.width = 2;
    style_skills.text.color = LV_COLOR_WHITE;

    // Released
    lv_style_copy(&style_red_released, &lv_style_plain);
    style_red_released.body.main_color = LV_COLOR_MAKE(48, 48, 48);
    style_red_released.body.grad_color = LV_COLOR_MAKE(48, 48, 48);
    style_red_released.body.border.color = LV_COLOR_RED;
    style_red_released.body.border.width = 2;
    style_red_released.text.color = LV_COLOR_WHITE;

    lv_style_copy(&style_blue_released, &lv_style_plain);
    style_blue_released.body.main_color = LV_COLOR_MAKE(48, 48, 48);
    style_blue_released.body.grad_color = LV_COLOR_MAKE(48, 48, 48);
    style_blue_released.body.border.color = LV_COLOR_BLUE;
    style_blue_released.body.border.width = 2;
    style_blue_released.text.color = LV_COLOR_WHITE;

    lv_style_copy(&style_skills_released, &lv_style_plain);
    style_skills_released.body.main_color = LV_COLOR_MAKE(48, 48, 48);
    style_skills_released.body.grad_color = LV_COLOR_MAKE(48, 48, 48);
    style_skills_released.body.border.color = LV_COLOR_GRAY;
    style_skills_released.body.border.width = 2;
    style_skills_released.text.color = LV_COLOR_WHITE;

    pros::delay(200);

    scr = lv_cont_create(NULL, NULL);
    lv_scr_load(scr);

    tv = lv_tabview_create(scr, NULL);
    lv_obj_set_pos(tv, 0, 0);
    lv_obj_set_size(tv, 480, 250);
    lv_tabview_set_sliding(tv, false);

    tab1 = lv_tabview_add_tab(tv, "Red");
    tab2 = lv_tabview_add_tab(tv, "Blue");
    tab3 = lv_tabview_add_tab(tv, "Skills");
    tab4 = lv_tabview_add_tab(tv, "Sensor");
    tab5 = lv_tabview_add_tab(tv, "Settings");

    tabRed(tab1);
    tabBlue(tab2);
    tabSkills(tab3);
    tabSensor(tab4);
    tabSetting(tab5);

    lv_tabview_set_tab_act(tv, 3, false);

    isInitialized = true;
  }
}

void Display::cleanUp() {
  lv_obj_clean(lv_layer_sys());
}

void Display::tabRed(lv_obj_t * parent) {
  lv_obj_t * btnmr[50];

  int size = Auton.getSize(SLOT_RED);
  for(int i = 0; i < size; i++) {
    if((i + 1) % 2 == 1) {
      btnmr[i] = createButton(i, 0, (int)( i / 2 ) * 35 + 20, 220, 30, Auton.getName(1, i), parent, red_click_action);
    } else {
      btnmr[i] = createButton(i, 230, (int)( i / 2 ) * 35 + 20, 220, 30, Auton.getName(1, i), parent, red_click_action);
    }

    lv_btn_set_style(btnmr[i], LV_BTN_STYLE_PR, &style_red);
    lv_btn_set_style(btnmr[i], LV_BTN_STYLE_REL, &style_red_released);
  }
}

void Display::tabBlue(lv_obj_t * parent) {
  lv_obj_t * btnmb[50];

  int size = Auton.getSize(SLOT_BLUE);
  for(int i = 0; i < size; i++) {
    if((i + 1) % 2 == 1) {
      btnmb[i] = createButton(i, 0, (int)( i / 2 ) * 35 + 20, 220, 30, Auton.getName(2, i), parent, blue_click_action);
    } else {
      btnmb[i] = createButton(i, 230, (int)( i / 2 ) * 35 + 20, 220, 30, Auton.getName(2, i), parent, blue_click_action);
    }

    lv_btn_set_style(btnmb[i], LV_BTN_STYLE_PR, &style_blue);
    lv_btn_set_style(btnmb[i], LV_BTN_STYLE_REL, &style_blue_released);
  }
}

void Display::tabSkills(lv_obj_t * parent) {
  lv_obj_t * btnms[50];

  int size = Auton.getSize(SLOT_SKILLS);
  for(int i = 0; i < size; i++) {
    if((i + 1) % 2 == 1) {
      btnms[i] = createButton(i, 0, (int)( i / 2 ) * 35 + 20, 220, 30, Auton.getName(3, i), parent, skills_click_action);
    } else {
      btnms[i] = createButton(i, 230, (int)( i / 2 ) * 35 + 20, 220, 30, Auton.getName(3, i), parent, skills_click_action);
    }

    lv_btn_set_style(btnms[i], LV_BTN_STYLE_PR, &style_skills);
    lv_btn_set_style(btnms[i], LV_BTN_STYLE_REL, &style_skills_released);
  }
}

void Display::tabSensor(lv_obj_t * parent) {

}

void Display::tabSetting(lv_obj_t * parent) {
  lv_obj_t * resetGyro = createButton(1, 0, 20, 200, 40, "Reset Gyro", parent, btn_click_action, &style_skills, &style_skills_released);
  lv_obj_t * resetOdom = createButton(2, 0, 70, 200, 40, "Reset Odom", parent, btn_click_action, &style_skills, &style_skills_released);
  lv_obj_t * resetArm = createButton(3, 0, 120, 200, 40, "Reset Arm", parent, btn_click_action, &style_skills, &style_skills_released);
}

void Display::start(void *ignore) {
  pros::delay(500);
  Display* that = static_cast<Display*>(ignore);
  that -> run();
}

void Display::run() {
  int nowType, lastType, nowSlot, lastSlot;

  isRunning = true;

  while(isRunning) {

    nowType = Auton.getType();
    nowSlot = Auton.getSlot();

    if(nowType != lastType || nowSlot != lastSlot) {
      std::string temp = "Selected Autonomous: " + Auton.getName(nowType, nowSlot);
      lv_label_set_text(autonName, temp.c_str());

      lastType = nowType;
      lastSlot = nowSlot;
    }

    for(int i = 0; i < updateInfo.size(); i++) {
      if(updateInfo[i].labelObj == nullptr) break;

      std::string temp = "";

      switch(updateInfo[i].type) {
        case 'i': temp = std::to_string(*(int*)updateInfo[i].data); break;
        case 'd': temp = std::to_string(*(double*)updateInfo[i].data); break;
        case 'b': temp = std::to_string(*(bool*)updateInfo[i].data); break;
        default: temp = std::to_string(*(double*)updateInfo[i].data); break;
      }

      if(temp != updateInfo[i].last) {
        std::string update = updateInfo[i].name + ": " + temp;
        lv_label_set_text(updateInfo[i].labelObj, update.c_str());
        updateInfo[i].last = temp;
      }
    }

    remoteUpdate();

    // std::cout << "Display Running" << std::endl;

    pros::delay(10);
  }
}

void Display::stop() {
  isRunning = false;
}

Display& Display::addInfo(std::string name, char type, void *info) {
  updateInfo.push_back(::info());

  int current = updateInfo.size() - 1;

  updateInfo[current].labelObj = lv_label_create(lv_tabview_get_tab(tv, 3), nullptr);

  if((current + 1) % 2 == 1) lv_obj_set_pos(updateInfo[current].labelObj, 0, (int)( current / 2 ) * 20 + 20);
    else lv_obj_set_pos(updateInfo[current].labelObj, 250, (int)( current / 2 ) * 20 + 20);

  static lv_style_t infoStyle;
  lv_style_copy(&infoStyle, &lv_style_plain);
  infoStyle.text.color = LV_COLOR_WHITE;
  lv_obj_set_style(updateInfo[current].labelObj, &infoStyle);

  updateInfo[current].name = name;
  updateInfo[current].type = type;
  updateInfo[current].data = info;
  updateInfo[current].last = "";
  return *this;
}

void Display::setRemoteText(std::string text_) {
  setText = text_;
}

void Display::remoteUpdate() {
  if(setText != lastText) {
    io::master.set_text(0, 0, setText.c_str());
    lastText = setText;
  }
}

lv_obj_t * Display::createLabel(int x, int y, std::string text_, lv_obj_t * parent) {
  lv_obj_t * label = lv_label_create(parent, NULL);
  lv_obj_set_pos(label, x, y);
  lv_label_set_text(label, text_.c_str());

  return label;
}

lv_obj_t * Display::createButton(int id, int x, int y, int width, int height, std::string text, lv_obj_t * parent, lv_action_t action, lv_style_t * btn_pr, lv_style_t * btn_rel) {
  lv_obj_t * button = lv_btn_create(parent, NULL);
  lv_obj_set_pos(button, x, y);
  lv_obj_set_size(button, width, height);
  lv_obj_set_free_num(button, id);
  lv_btn_set_action(button, LV_BTN_ACTION_CLICK, action);

  if(btn_pr != nullptr || btn_rel != nullptr) {
    lv_btn_set_style(button, LV_BTN_STYLE_PR, btn_pr);
    lv_btn_set_style(button, LV_BTN_STYLE_REL, btn_rel);
  } else {
    lv_btn_set_style(button, LV_BTN_STYLE_PR, lv_theme_get_current()->btn.pr);
    lv_btn_set_style(button, LV_BTN_STYLE_REL, lv_theme_get_current()->btn.rel);
  }

  lv_obj_t * buttonLabel = createLabel(0, 0, text, button);
  lv_obj_align(buttonLabel, button, LV_ALIGN_CENTER, 0, 0);

  return button;
}
