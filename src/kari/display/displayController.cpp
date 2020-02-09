#include "main.h"

#include "kari/displayController.h"
#include "kari/autonController.h"
#include "kari/util/misc.h"

#include "kari/control/chassis.h"
#include "kari/control/arm.h"

static Autonomous Auton;

static Chassis chassis;
static Arm arm;

bool Display::isRunning = false,
Display::isInitialized = false;

int Display::currScr = 1;

std::string Display::setText = "",
Display::lastText = "";

std::vector<info> Display::updateInfo;

static int auton_type = 1;

static lv_style_t style_red;
static lv_style_t style_blue;
static lv_style_t style_skills;

static lv_style_t style_red_released;
static lv_style_t style_blue_released;
static lv_style_t style_skills_released;

static lv_obj_t * scr;

static lv_res_t btn_click_action(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);

  

  return LV_RES_OK;
}

static lv_res_t auton_click_action(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);

  Auton.setType(auton_type);
  Auton.setSlot(id);

  return LV_RES_OK;
}

Display::Display() {
  if(!isInitialized) {
    // Theme & Style init
    lv_theme_t * th = lv_theme_material_init(120, NULL);

    // th->tabview.btn.tgl_rel->body.main_color = LV_COLOR_RED;

    lv_theme_set_current(th);

    lv_style_plain.body.radius = 1;

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

    lv_obj_t * tv = lv_tabview_create(scr, NULL);
    lv_obj_set_pos(tv, 0, 0);
    lv_obj_set_size(tv, 480, 250);
    lv_tabview_set_sliding(tv, false);

    lv_obj_t * tab1 = lv_tabview_add_tab(tv, "Red");
    lv_obj_t * tab2 = lv_tabview_add_tab(tv, "Blue");
    lv_obj_t * tab3 = lv_tabview_add_tab(tv, "Skills");
    lv_obj_t * tab4 = lv_tabview_add_tab(tv, "Sensor");
    lv_obj_t * tab5 = lv_tabview_add_tab(tv, "Settings");

    tabRed(tab1);
    tabBlue(tab2);
    tabSkills(tab3);
    tabSensor(tab4);
    tabSetting(tab5);

    isInitialized = true;
  }
}

void Display::tabRed(lv_obj_t * parent) {
  lv_obj_t * btnmr[50];

  int size = Auton.getSize(SLOT_RED);
  for(int i = 0; i < size; i++) {
    if((i + 1) % 2 == 1) {
      btnmr[i] = createButton(i, 0, (int)( i / 2 ) * 35 + 20, 220, 30, Auton.getName(1, i), parent, auton_click_action);
    } else {
      btnmr[i] = createButton(i, 230, (int)( i / 2 ) * 35 + 20, 220, 30, Auton.getName(1, i), parent, auton_click_action);
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
      btnmb[i] = createButton(i, 0, (int)( i / 2 ) * 35 + 20, 220, 30, Auton.getName(2, i), parent, auton_click_action);
    } else {
      btnmb[i] = createButton(i, 230, (int)( i / 2 ) * 35 + 20, 220, 30, Auton.getName(2, i), parent, auton_click_action);
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
      btnms[i] = createButton(i, 0, (int)( i / 2 ) * 35 + 20, 220, 30, Auton.getName(3, i), parent, auton_click_action);
    } else {
      btnms[i] = createButton(i, 230, (int)( i / 2 ) * 35 + 20, 220, 30, Auton.getName(3, i), parent, auton_click_action);
    }

    lv_btn_set_style(btnms[i], LV_BTN_STYLE_PR, &style_skills);
    lv_btn_set_style(btnms[i], LV_BTN_STYLE_REL, &style_skills_released);
  }
}

void Display::tabSensor(lv_obj_t * parent) {
  // lv_obj_t * yeet = createLabel(20, 0, "Cyka blyat", parent);
  for(int i = 0; i < updateInfo.size(); i++) {
    updateInfo[i].labelObj = createLabel(20, 100, updateInfo[i].name + ": ", parent);
    // updateInfo[i].labelObj = createLabel(180, 200, updateInfo[i].name + ": ", parent);
  }
}

void Display::tabSetting(lv_obj_t * parent) {

}

void Display::start(void *ignore) {
  pros::delay(500);
  Display* that = static_cast<Display*>(ignore);
  that -> run();
}

void Display::run() {
  // int nowType, lastType, nowSlot, lastSlot;

  while(isRunning) {

    switch(currScr) {

      case 1: { // Auton
        break;
      }

      case 2: { // Sensor
        for(int i = 0; i < updateInfo.size(); i++) {
          if(updateInfo[i].labelObj == nullptr) break;

          std::string temp = "";

          switch(updateInfo[i].type) {
            case 'i': temp = std::to_string(*(int*)updateInfo[i].data);
            case 'd': temp = std::to_string(*(double*)updateInfo[i].data);
            case 'b': temp = std::to_string(*(bool*)updateInfo[i].data);
          }

          if(temp != updateInfo[i].last) {
            std::string update = updateInfo[i].name + ": " + temp;
            lv_label_set_text(updateInfo[i].labelObj, update.c_str());
            updateInfo[i].last = temp;
          }
        }
        break;
      }

      case 3: { // Settings
        break;
      }

      default: {
        break;
      }
    }

    // nowType = Auton.getType();
    // nowSlot = Auton.getSlot();
    // if(lastType != nowType || lastSlot != nowSlot) {
    //   name.erase(name.begin() + 16, name.end());
    //   name.append(Auton.getName(Auton.getType(), Auton.getSlot()));
    //   c = name.c_str();
    //   lv_label_set_text(autonStat, c);
    //
    //   switch(Auton.getType()) {
    //     case SLOT_RED: {
    //       overlay.body.main_color = LV_COLOR_RED;
    //       overlay.body.grad_color = LV_COLOR_RED;
    //       break;
    //     }
    //
    //     case SLOT_BLUE: {
    //       overlay.body.main_color = LV_COLOR_BLUE;
    //       overlay.body.grad_color = LV_COLOR_BLUE;
    //       break;
    //     }
    //
    //     case SLOT_SKILLS: {
    //       overlay.body.main_color = LV_COLOR_GRAY;
    //       overlay.body.grad_color = LV_COLOR_GRAY;
    //       break;
    //     }
    //
    //     default: {
    //       overlay.body.main_color = LV_COLOR_BLACK;
    //       overlay.body.grad_color = LV_COLOR_BLACK;
    //       break;
    //     }
    //   }
    // }
    //
    // lastType = nowType;
    // lastSlot = nowSlot;

    remoteUpdate();

    pros::delay(10);
  }
}

void Display::stop() {
  isRunning = false;
}

Display& Display::addInfo(std::string name, char type, void *info) {
  updateInfo.push_back(::info());
  updateInfo[updateInfo.size() - 1].labelObj = nullptr;
  updateInfo[updateInfo.size() - 1].name = name;
  updateInfo[updateInfo.size() - 1].type = type;
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
