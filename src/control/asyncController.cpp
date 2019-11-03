#include "main.h"

#include "config/motor.h"
#include "config/io.h"

#include "control/asyncController.h"
#include "control/drive.h"
#include "control/macro.h"

bool ControlAsync::isRunning = false;
bool ControlAsync::isPaused = false;

bool ControlAsync::isDrive = false,
ControlAsync::isTurn = false,
ControlAsync::isStrafe = false,
ControlAsync::isRack = false,
ControlAsync::isArm = false,
ControlAsync::isZeroing = false,
ControlAsync::isDown = false;

bool ControlAsync::usingGyro = false;

int ControlAsync::sturn = 0;

PIDS ControlAsync::chassisVar = {0, 0, 0, 0, 0};
PIDS ControlAsync::rackVar = {0, 0, 0, 0, 0};
PIDS ControlAsync::armVar = {0, 0, 0, 0, 0};

Vector2 ControlAsync::chassis_target = {0, 0, 0};
Vector2 ControlAsync::rack_target = {0, 0, 0};
Vector2 ControlAsync::arm_target = {0, 0, 0};

double ControlAsync::chassis_kP = 0.6, ControlAsync::chassis_kD = 0.6;
double ControlAsync::rack_kP = 0.11;
double ControlAsync::arm_kP = 210;

ControlAsync::ControlAsync() {}

void ControlAsync::run(void *args) {
  pros::delay(500);
  ControlAsync* that = static_cast<ControlAsync*>(args);
  that -> update();
}

void ControlAsync::pause() {
  isDrive = false;
  isTurn = false;
  isStrafe = false;
  isRack = false;
  isArm = false;
  reset_drive();
  reset_rack();
  reset_arm();
  isPaused = true;
}

void ControlAsync::resume() { isPaused = false; }

void ControlAsync::stop() { isRunning = false; }

void ControlAsync::update() {

  isRunning = true;

  reset_drive();
  reset_rack();
  reset_arm();

  double deltaL, deltaR;

  double tolerance = 3;

  while(isRunning) {
    if(isPaused) { pros::delay(20); continue; }

    /*===========================================
      RACK
    ===========================================*/

    if(isRack) {
      if(rack_target.length > rackPot.get_value()) { // Goin' up
        rackVar.output = pTerm(rack_target.length, rackPot.get_value(), rack_kP);

        if(rackVar.output > rackVar.slewOutput + rack_target.rate) {
          rackVar.slewOutput += rack_target.rate;
        } else {
          rackVar.slewOutput = rackVar.output;
        }

        if(rackVar.slewOutput > rack_target.speed) rackVar.slewOutput = rack_target.speed;

        ::rack(rackVar.slewOutput);

        if(isSettled(rackVar.output, tolerance)) { reset_rack(); isRack = false; }
      }

      if(rack_target.length < rackPot.get_value()) { // Goin' down
        rackVar.output = pTerm(rack_target.length, rackPot.get_value(), rack_kP);

        if(abs(rackVar.output) > rackVar.slewOutput + rack_target.rate) {
          rackVar.slewOutput += rack_target.rate;
        } else {
          rackVar.slewOutput = abs(rackVar.output);
        }

        if(rackVar.slewOutput > rack_target.speed) rackVar.slewOutput = rack_target.speed;

        ::rack(-rackVar.slewOutput);

        if(isSettled(rackVar.output, tolerance)) { reset_rack(); isRack = false; }
      }
    }

    /*===========================================
      ARM
    ===========================================*/

    if(isArm) {
      if(arm_target.length > Arm.get_position()) { // Goin' up
        armVar.output = pTerm(arm_target.length, Arm.get_position(), arm_kP);

        if(armVar.output > armVar.slewOutput + arm_target.rate) {
          armVar.slewOutput += arm_target.rate;
        } else {
          armVar.slewOutput = armVar.output;
        }

        if(armVar.slewOutput > arm_target.speed) armVar.slewOutput = arm_target.speed;

        ::arm(armVar.slewOutput);

        if(isSettled(armVar.output, tolerance)) { reset_arm(); isArm = false; }
      }

      if(arm_target.length < Arm.get_position()) { // Goin' down
        armVar.output = pTerm(arm_target.length, Arm.get_position(), arm_kP);

        if(abs(armVar.output) > armVar.slewOutput + arm_target.rate) {
          armVar.slewOutput += arm_target.rate;
        } else {
          armVar.slewOutput = abs(armVar.output);
        }

        if(armVar.slewOutput > arm_target.speed) armVar.slewOutput = arm_target.speed;

        ::arm(-armVar.slewOutput);

        if(isSettled(armVar.output, tolerance)) { reset_arm(); isArm = false; }
      }
    }

    /*===========================================
      ZEROING ARM
    ===========================================*/

    if(isZeroing) {
      Arm.set_current_limit(10000);
      ::arm(-200);

      if(armLimit.get_new_press()) {
        ::arm(0);
        Arm.tare_position();
        isZeroing = false;
      } else if(isDown) { ::arm(0); isZeroing = false; }
    }

    /*===========================================
      DRIVING
    ===========================================*/

    if(isDrive) {
      if(chassis_target.length > 0) {
        deltaL = (LF.get_position() + LB.get_position()) / 2;
        deltaR = (RF.get_position() + RB.get_position()) / 2;
        chassisVar.current = ( deltaL + deltaR ) / 2;
        chassisVar.error = chassis_target.length - chassisVar.current;

        chassisVar.output = pTerm(chassis_target.length, chassisVar.current, chassis_kP) + dTerm(chassisVar.error, chassisVar.last) * chassis_kD;

        chassisVar.last = chassisVar.error;

        if(chassisVar.output > chassisVar.slewOutput + chassis_target.rate) {
          chassisVar.slewOutput += chassis_target.rate;
        } else {
          chassisVar.slewOutput = chassisVar.output;
        }

        if(chassisVar.slewOutput > chassis_target.speed) chassisVar.slewOutput = chassis_target.speed;

        left(chassisVar.slewOutput - slop(0));
        right(chassisVar.slewOutput + slop(0));

        if(isSettled(chassisVar.error, 9)) { reset_drive(); isDrive = false; }
      }

      if(chassis_target.length < 0) {
        deltaL = (LF.get_position() + LB.get_position()) / 2;
        deltaR = (RF.get_position() + RB.get_position()) / 2;
        chassisVar.current = ( deltaL + deltaR ) / 2;
        chassisVar.error = chassis_target.length - chassisVar.current;

        chassisVar.output = pTerm(chassis_target.length, chassisVar.current, chassis_kP) + dTerm(chassisVar.error, chassisVar.last) * chassis_kD;

        chassisVar.last = chassisVar.error;

        if(abs(chassisVar.output) > chassisVar.slewOutput + chassis_target.rate) {
          chassisVar.slewOutput += chassis_target.rate;
        } else {
          chassisVar.slewOutput = abs(chassisVar.output);
        }

        if(chassisVar.slewOutput > chassis_target.speed) chassisVar.slewOutput = chassis_target.speed;

        left(-chassisVar.slewOutput - slop(0));
        right(-chassisVar.slewOutput + slop(0));

        if(isSettled(abs(chassisVar.error), 9)) { reset_drive(); isDrive = false; }
      }
    }

    /*===========================================
      TURNING
    ===========================================*/

    if(isTurn) {
      if(chassis_target.length > 0) { // Turn Right
        if(!usingGyro) {
          deltaL = (LF.get_position() + LB.get_position()) / 2;
          deltaR = (RF.get_position() + RB.get_position()) / 2;
          chassisVar.current = ( deltaL + abs(deltaR) ) / 2;
        } else {
          chassisVar.current = Gyro.get_value();
        }

        chassisVar.error = chassis_target.length - chassisVar.current;

        chassisVar.output = pTerm(chassis_target.length, chassisVar.current, chassis_kP) + dTerm(chassisVar.error, chassisVar.last) * chassis_kD;

        chassisVar.last = chassisVar.error;

        if(chassisVar.output > chassisVar.slewOutput + chassis_target.rate) {
          chassisVar.slewOutput += chassis_target.rate;
        } else {
          chassisVar.slewOutput = chassisVar.output;
        }

        if(chassisVar.slewOutput > chassis_target.speed) chassisVar.slewOutput = chassis_target.speed;

        left(chassisVar.slewOutput);
        right(-chassisVar.slewOutput);

        if(isSettled(chassisVar.error, 6)) { reset_drive(); isTurn = false; usingGyro = false; }
      }

      if(chassis_target.length < 0) { // Turn Left
        if(!usingGyro) {
          deltaL = (LF.get_position() + LB.get_position()) / 2;
          deltaR = (RF.get_position() + RB.get_position()) / 2;
          chassisVar.current = ( abs(deltaL) + deltaR ) / 2;
        } else {
          chassisVar.current = Gyro.get_value();
        }

        chassisVar.error = chassis_target.length + chassisVar.current;

        chassisVar.output = pTerm(chassis_target.length, -chassisVar.current, chassis_kP) + dTerm(chassisVar.error, chassisVar.last) * chassis_kD;

        chassisVar.last = chassisVar.error;

        if(abs(chassisVar.output) > chassisVar.slewOutput + chassis_target.rate) {
          chassisVar.slewOutput += chassis_target.rate;
        } else {
          chassisVar.slewOutput = abs(chassisVar.output);
        }

        if(chassisVar.slewOutput > chassis_target.speed) chassisVar.slewOutput = chassis_target.speed;

        left(-chassisVar.slewOutput);
        right(chassisVar.slewOutput);

        if(isSettled(chassisVar.error, 6)) { reset_drive(); isTurn = false; usingGyro = false; }
      }

      if(chassis_target.length == 0) { // Hold Angle
        chassisVar.current = Gyro.get_value();
        chassisVar.error = chassis_target.length - chassisVar.current;

        chassisVar.output = pTerm(chassis_target.length, chassisVar.current, 0.5) + dTerm(chassisVar.error, chassisVar.last) * 1;

        chassisVar.last = chassisVar.error;

        chassisVar.slewOutput = chassisVar.output;

        if(chassisVar.slewOutput > chassis_target.speed) chassisVar.slewOutput = chassis_target.speed;

        left(chassisVar.slewOutput);
        right(-chassisVar.slewOutput);
      }
    }

    /*===========================================
      STRAFING
    ===========================================*/

    if(isStrafe) {
      if(chassis_target.length > 0) {
        deltaL = (LF.get_position() - LB.get_position()) / 2;
        deltaR = (RB.get_position() - RF.get_position()) / 2;
        chassisVar.current = (deltaL + deltaR) / 2;
        chassisVar.error = chassis_target.length - chassisVar.current;

        chassisVar.output = pTerm(chassis_target.length, chassisVar.current, chassis_kP) + dTerm(chassisVar.error, chassisVar.last) * chassis_kD;

        chassisVar.last = chassisVar.error;

        if(chassisVar.output > chassisVar.slewOutput + chassis_target.rate) {
          chassisVar.slewOutput += chassis_target.rate;
        } else {
          chassisVar.slewOutput = chassisVar.output;
        }

        if(chassisVar.slewOutput > chassis_target.speed) chassisVar.slewOutput = chassis_target.speed;

        LF.move_velocity(-chassisVar.slewOutput - slop(2, sturn, 0));
        LB.move_velocity(chassisVar.slewOutput + slop(2, sturn, 0));
        RF.move_velocity(-chassisVar.slewOutput - slop(2, sturn, 0));
        RB.move_velocity(chassisVar.slewOutput + slop(2, sturn, 0));

        if(isSettled(chassisVar.error, 6)) { reset_drive(); isStrafe = false; }
      }

      if(chassis_target.length < 0) {
        deltaL = (LF.get_position() - LB.get_position()) / 2;
        deltaR = (RB.get_position() - RF.get_position()) / 2;
        chassisVar.current = (deltaL + deltaR) / 2;
        chassisVar.error = chassis_target.length - chassisVar.current;

        chassisVar.output = pTerm(chassis_target.length, chassisVar.current, chassis_kP) + dTerm(chassisVar.error, chassisVar.last) * chassis_kD;

        chassisVar.last = chassisVar.error;

        if(abs(chassisVar.output) > chassisVar.slewOutput + chassis_target.rate) {
          chassisVar.slewOutput += chassis_target.rate;
        } else {
          chassisVar.slewOutput = abs(chassisVar.output);
        }

        if(chassisVar.slewOutput > chassis_target.speed) chassisVar.slewOutput = chassis_target.speed;

        LF.move_velocity(-chassisVar.slewOutput - slop(2, -sturn, 0));
        LB.move_velocity(chassisVar.slewOutput + slop(2, -sturn, 0));
        RF.move_velocity(-chassisVar.slewOutput - slop(2, -sturn, 0));
        RB.move_velocity(chassisVar.slewOutput + slop(2, -sturn, 0));

        if(isSettled(abs(chassisVar.error), 6)) { reset_drive(); isStrafe = false; }
      }
    }

    ::wait(20);
  }

  print("Async Controller Terminated");
}

void ControlAsync::reset_drive() {
  chassisVar = {0, 0, 0, 0, 0};

  LF.tare_position();
  LB.tare_position();
  RF.tare_position();
  RB.tare_position();

  left(0);
  right(0);

  chassis_target = {0, 0, 0};
  sturn = 0;
}

void ControlAsync::reset_rack() {
  rackVar = {0, 0, 0, 0, 0};
  ::rack(0);
  rack_target = {0, 0, 0};
}

void ControlAsync::reset_arm() {
  armVar = {0, 0, 0, 0, 0};
  ::arm(0);
  arm_target = {0, 0, 0};
}

bool ControlAsync::isDisabled() {
  if(isDrive || isTurn || isStrafe) {
    return false;
  } else {
    return true;
  }
}

void ControlAsync::disable_drive() {
  isDrive = false;
  isTurn = false;
  isStrafe = false;
}

void ControlAsync::disable_rack() {
  isRack = false;
}

void ControlAsync::disable_arm() {
  isArm = false;
}

void ControlAsync::zero_arm() {
  isArm = false;
  reset_arm();
  isDown = armLimit.get_value();
  isZeroing = true;
}

void ControlAsync::drive(double length, int speed, int rate) {
  reset_drive();
  this -> chassis_target.length = length;
  this -> chassis_target.speed = speed;
  this -> chassis_target.rate = rate;
  isDrive = true;
}

void ControlAsync::turn(double length, int speed, int rate) {
  reset_drive();
  this -> chassis_target.length = length;
  this -> chassis_target.speed = speed;
  this -> chassis_target.rate = rate;

  if(usingGyro) Gyro.reset();
  isTurn = true;
}

void ControlAsync::strafe(double length, int speed, int rate) {
  reset_drive();
  this -> chassis_target.length = length;
  this -> chassis_target.speed = speed;
  this -> chassis_target.rate = rate;
  isStrafe = true;
}

void ControlAsync::hold_angle() {
  reset_drive();
  this -> chassis_target.length = 0;
  this -> chassis_target.speed = 100;
  this -> chassis_target.rate = 9;
  Gyro.reset();
  isTurn = true;
}

ControlAsync& ControlAsync::withGyro() {
  usingGyro = true;
  return *this;
}

ControlAsync& ControlAsync::withSturn(int sturn) {
  this -> sturn = sturn;
  return *this;
}

ControlAsync& ControlAsync::withConst(int mode, double kP_, double kD_) {
  switch(mode) {
    case MODE_CHASSIS: {
      chassis_kP = kP_;
      chassis_kD = kD_;
    }

    case MODE_RACK: {
      rack_kP = kP_;
    }

    case MODE_ARM: {
      arm_kP = kP_;
    }

    default: {
      print("Error while setting kP for thread");
    }
  }

  return *this;
}

void ControlAsync::rack(double length, int speed, int rate) {
  reset_rack();
  this -> rack_target.length = length;
  this -> rack_target.speed = speed;
  this -> rack_target.rate = rate;
  isRack = true;
}

void ControlAsync::arm(double length, int speed, int rate) {
  reset_arm();
  this -> arm_target.length = length;
  this -> arm_target.speed = speed;
  this -> arm_target.rate = rate;
  isArm = true;
}
