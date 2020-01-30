#include "main.h"

#include "kari/displayController.h"
#include "kari/autonController.h"
#include "kari/util/misc.h"

#include "kari/control/chassis.h"
#include "kari/control/arm.h"

static Autonomous Auton;

static Chassis chassis;
static Arm arm;

std::string Display::setText = "",
Display::lastText = "";

std::vector<info> Display::updateInfo;

static int screen = 0;
static int auton_type = 1;

static lv_style_t overlay;
static lv_style_t mainScr;

static lv_style_t style_red;
static lv_style_t style_blue;
static lv_style_t style_skills;

static lv_style_t style_red_released;
static lv_style_t style_blue_released;
static lv_style_t style_skills_released;

static lv_obj_t * scr;

static lv_obj_t * loading;
static lv_obj_t * loader;

static bool initialized = false;

static lv_obj_t * autonStat;
static lv_obj_t * status;
static lv_obj_t * btnBack;

static lv_obj_t * btnRed;
static lv_obj_t * btnBlue;
static lv_obj_t * btnSkills;

static lv_res_t main_click_action(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);

  lv_page_clean(scr);

  Display display;

  if(id == 1) { display.auton(); }
  if(id == 2) { display.sensor(); }
  if(id == 3) { display.camera(); }
  if(id == 4) { display.setting(); }

  // Home Button
  if(id == 420) { display.main(); }

  return LV_RES_OK;
}

static lv_res_t auton_click_action(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);

  Auton.setType(auton_type);
  Auton.setSlot(id);

  return LV_RES_OK;
}

static lv_res_t settings_click_action(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);

  switch(id) {
    case 1: arm.reset(); break;
    case 2: chassis.calibrateGyro(); break;
  }

  return LV_RES_OK;
}

static lv_res_t system_action(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);

  lv_page_clean(scr);

  Display display;

  switch(id) {
    case 1: auton_type = 1; display.auton(); break;
    case 2: auton_type = 2; display.auton(); break;
    case 3: auton_type = 3; display.auton(); break;

    default: auton_type = 1; display.auton(); break;
  }

  return LV_RES_OK;
}

Display::Display() {
  if(!initialized) {
    // Theme & Style init
    lv_theme_t * th = lv_theme_material_init(120, NULL);
    lv_theme_set_current(lv_theme_get_material());

    lv_style_plain.body.radius = 1;

    lv_style_copy(&overlay, &lv_style_plain);
    overlay.body.main_color = LV_COLOR_RED;
    overlay.body.grad_color = LV_COLOR_RED;
    overlay.body.border.color = LV_COLOR_BLACK;
    overlay.body.border.width = 2;
    overlay.text.color = LV_COLOR_WHITE;

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

    // Overlay & Screen setup
    status = lv_cont_create(lv_layer_top(), NULL);
    lv_obj_set_style(status, &overlay);
    lv_obj_set_pos(status, 2, 2);
    lv_obj_set_size(status, 476, 20);
    lv_cont_set_layout(status, LV_LAYOUT_CENTER);

    autonStat = lv_label_create(status, NULL);
    lv_obj_set_style(autonStat, &overlay);
    lv_obj_set_y(autonStat, 2);

    btnBack = createButton(420, 5, 190, 100, 40, SYMBOL_HOME" Home", lv_layer_top(), main_click_action);

    btnRed = createButton(1, 5, 35, 80, 40, "Red", lv_layer_top(), system_action);
    lv_btn_set_style(btnRed, LV_BTN_STYLE_PR, &style_red);
    lv_btn_set_style(btnRed, LV_BTN_STYLE_REL, &style_red_released);

    btnBlue = createButton(2, 5, 80, 80, 40, "Blue", lv_layer_top(), system_action);
    lv_btn_set_style(btnBlue, LV_BTN_STYLE_PR, &style_blue);
    lv_btn_set_style(btnBlue, LV_BTN_STYLE_REL, &style_blue_released);

    btnSkills = createButton(3, 5, 125, 80, 40, "Skills", lv_layer_top(), system_action);
    lv_btn_set_style(btnSkills, LV_BTN_STYLE_PR, &style_skills);
    lv_btn_set_style(btnSkills, LV_BTN_STYLE_REL, &style_skills_released);

    initialized = true;

    pros::delay(200);

    scr = lv_page_create(NULL, NULL);
    main();
  }
}

void Display::auton() {
  screen = 1;

  lv_obj_set_x(btnBack, 5);
  lv_obj_set_x(btnRed, 5);
  lv_obj_set_x(btnBlue, 5);
  lv_obj_set_x(btnSkills, 5);

  lv_obj_t * btnm[50];

  switch(auton_type) {
    case SLOT_RED: {
      int size = Auton.getSize(SLOT_RED);
      for(int i = 0; i < size; i++) {
        if(i == 0) {
          btnm[i] = createButton(i, 200, 40, 250, 30, Auton.getName(auton_type, i), scr, auton_click_action);
        } else {
          btnm[i] = createButton(i, 200, i * 35 + 20, 250, 30, Auton.getName(auton_type, i), scr, auton_click_action);
        }

        lv_btn_set_style(btnm[i], LV_BTN_STYLE_PR, &style_red);
        lv_btn_set_style(btnm[i], LV_BTN_STYLE_REL, &style_red_released);
      }

      break;
    }

    case SLOT_BLUE: {
      int size = Auton.getSize(SLOT_BLUE);
      for(int i = 0; i < size; i++) {
        if(i == 0) {
          btnm[i] = createButton(i, 200, 40, 250, 30, Auton.getName(auton_type, i), scr, auton_click_action);
        } else {
          btnm[i] = createButton(i, 200, i * 35 + 20, 250, 30, Auton.getName(auton_type, i), scr, auton_click_action);
        }

        lv_btn_set_style(btnm[i], LV_BTN_STYLE_PR, &style_blue);
        lv_btn_set_style(btnm[i], LV_BTN_STYLE_REL, &style_blue_released);
      }

      break;
    }

    case SLOT_SKILLS: {
      int size = Auton.getSize(SLOT_SKILLS);
      for(int i = 0; i < size; i++) {
        if(i == 0) {
          btnm[i] = createButton(i, 200, 40, 250, 30, Auton.getName(auton_type, i), scr, auton_click_action);
        } else {
          btnm[i] = createButton(i, 200, i * 35 + 20, 250, 30, Auton.getName(auton_type, i), scr, auton_click_action);
        }

        lv_btn_set_style(btnm[i], LV_BTN_STYLE_PR, &style_skills);
        lv_btn_set_style(btnm[i], LV_BTN_STYLE_REL, &style_skills_released);
      }

      break;
    }

    default: {
      int size = Auton.getSize(SLOT_RED);
      for(int i = 0; i < size; i++) {
        if(i == 0) {
          btnm[i] = createButton(i, 200, 40, 250, 30, Auton.getName(auton_type, i), scr, auton_click_action);
        } else {
          btnm[i] = createButton(i, 200, i * 35 + 20, 250, 30, Auton.getName(auton_type, i), scr, auton_click_action);
        }

        lv_btn_set_style(btnm[i], LV_BTN_STYLE_PR, &style_red);
        lv_btn_set_style(btnm[i], LV_BTN_STYLE_REL, &style_red_released);
      }

      break;
    }
  }

  lv_scr_load(scr);
}

void Display::sensor() {
  screen = 2;
  lv_obj_set_x(btnBack, 5);

  for(int i = 0; i < updateInfo.size(); i++) {
    if(i == 0) updateInfo[i].labelObj = createLabel(20, 50, updateInfo[i].text, scr);
      else updateInfo[i].labelObj = createLabel(20, i * 20 + 20, updateInfo[i].text, scr);
  }

  lv_scr_load(scr);
}

void Display::camera() {
  screen = 3;
  lv_obj_set_x(btnBack, 5);

  lv_scr_load(scr);
}

void Display::setting() {
  screen = 4;

  lv_obj_set_x(btnBack, 5);

  // lv_obj_t * arm_reset = createButton(1, 0, 0, 200, 50, "Calibrate Arm", scr, settings_click_action);
  // lv_obj_t * gyro_reset = createButton(2, 0, 80, 200, 50, "Calibrate Gyro", scr, settings_click_action);

  lv_obj_t * hotSauce = lv_img_create(scr, NULL);
  lv_img_set_src(hotSauce, &michael1);
  lv_obj_set_size(hotSauce, 240, 240);
  lv_obj_set_pos(hotSauce, 240, 0);

  lv_scr_load(scr);
}

void Display::run() {
  int nowType, lastType, nowSlot, lastSlot;
  std::string name;
  const char * c;

  name = "Auton Selected: ";

  while(true) {

    switch(screen) {

      case 1: { // Auton
        break;
      }

      case 2: { // Sensor
        for(int i = 0; i < updateInfo.size(); i++) {
          if(updateInfo[i].labelObj == nullptr) break;
          if(*(double*)updateInfo[i].data != updateInfo[i].last) {
            std::string temp = updateInfo[i].text + ": " + std::to_string(*(double*)updateInfo[i].data);
            lv_label_set_text(updateInfo[i].labelObj, temp.c_str());
            updateInfo[i].last = *(double*)updateInfo[i].data;
          }
        }
        break;
      }

      case 3: { // Camera
        break;
      }

      case 4: { // Settings
        break;
      }

      default: {
        break;
      }
    }

    // Auton name display
    nowType = Auton.getType();
    nowSlot = Auton.getSlot();
    if(lastType != nowType || lastSlot != nowSlot) {
      name.erase(name.begin() + 16, name.end());
      name.append(Auton.getName(Auton.getType(), Auton.getSlot()));
      c = name.c_str();
      lv_label_set_text(autonStat, c);

      switch(Auton.getType()) {
        case SLOT_RED: {
          overlay.body.main_color = LV_COLOR_RED;
          overlay.body.grad_color = LV_COLOR_RED;
          break;
        }

        case SLOT_BLUE: {
          overlay.body.main_color = LV_COLOR_BLUE;
          overlay.body.grad_color = LV_COLOR_BLUE;
          break;
        }

        case SLOT_SKILLS: {
          overlay.body.main_color = LV_COLOR_GRAY;
          overlay.body.grad_color = LV_COLOR_GRAY;
          break;
        }

        default: {
          overlay.body.main_color = LV_COLOR_BLACK;
          overlay.body.grad_color = LV_COLOR_BLACK;
          break;
        }
      }
    }

    lastType = nowType;
    lastSlot = nowSlot;

    remoteUpdate();

    pros::delay(10);
  }
}

void Display::start(void *ignore) {
  pros::delay(500);
  Display* that = static_cast<Display*>(ignore);
  that -> run();
}

Display& Display::addInfo(std::string text, void *info) {
  updateInfo.push_back(::info());
  updateInfo[updateInfo.size() - 1].labelObj = nullptr;
  updateInfo[updateInfo.size() - 1].text = text;
  updateInfo[updateInfo.size() - 1].data = info;
  updateInfo[updateInfo.size() - 1].last = 1;
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

lv_obj_t * Display::createButton(int id, int x, int y, int width, int height, std::string text, lv_obj_t * parent, lv_action_t action) {
  lv_obj_t * button = lv_btn_create(parent, NULL);
  lv_obj_set_pos(button, x, y);
  lv_obj_set_size(button, width, height);
  lv_obj_set_free_num(button, id);
  lv_btn_set_action(button, LV_BTN_ACTION_CLICK, action);
  lv_obj_t * buttonLabel = createLabel(0, 0, text, button);
  lv_obj_align(buttonLabel, button, LV_ALIGN_CENTER, 0, 0);

  return button;
}
