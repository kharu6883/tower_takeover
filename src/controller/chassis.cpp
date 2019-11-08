#include "controller/chassis.h"

Chassis::Chassis(double kP_, double kD_) : kP(kP_), kD(kD_),
LF(LFPORT, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS),
LB(LBPORT, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS),
RF(RFPORT, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS),
RB(RBPORT, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_ROTATIONS) { }

Chassis::~Chassis() {
  reset();
}

Chassis& Chassis::withTolerance(double tolerance_) {
  tolerance = tolerance_;
  return *this;
}

Chassis& Chassis::withSlop(double amp_, double offset_) {
  amp = amp_;
  offset = offset_;
  return *this;
}

Chassis& Chassis::drive(double target_, int speed_, int rate_) {
  target = target_;
  speed = speed_;
  rate = rate_;
  mode = DRIVING;
  return *this;
}

Chassis& Chassis::turn(double target_, int speed_, int rate_) {
  target = target_;
  speed = speed_;
  rate = rate_;
  mode = TURNING;
  return *this;
}

Chassis& Chassis::strafe(double target_, int speed_, int rate_) {
  target = target_;
  speed = speed_;
  rate = rate_;
  mode = STRAFING;
  return *this;
}

Chassis& Chassis::align(double target_) {
  target = target_;
  mode = ALIGNING;
  return *this;
}

void Chassis::waitUntilSettled() {
  while(!isSettled) pros::delay(20);
}

void Chassis::reset() {
  current = last = error = derivative = output = slewOutput = 0;

  mode = IDLE;

  LF.move_velocity(0);
  LB.move_velocity(0);
  RF.move_velocity(0);
  RB.move_velocity(0);

  LF.tare_position();
  LB.tare_position();
  RF.tare_position();
  RB.tare_position();
}

void Chassis::lock() {
  LF.set_brake_mode(MOTOR_BRAKE_HOLD);
  LB.set_brake_mode(MOTOR_BRAKE_HOLD);
  RF.set_brake_mode(MOTOR_BRAKE_HOLD);
  RB.set_brake_mode(MOTOR_BRAKE_HOLD);
}

void Chassis::unlock() {
  LF.set_brake_mode(MOTOR_BRAKE_COAST);
  LB.set_brake_mode(MOTOR_BRAKE_COAST);
  RF.set_brake_mode(MOTOR_BRAKE_COAST);
  RB.set_brake_mode(MOTOR_BRAKE_COAST);
}

bool Chassis::getState() {
  return isSettled;
}

int Chassis::getMode() {
  return mode;
}

void Chassis::run() {
  isRunning = true;

  while(isRunning) {

    switch(mode) {
      case 1: { // Driving
        deltaL = ( LF.get_position() + LB.get_position() ) / 2;
        deltaR = ( RF.get_position() + RB.get_position() ) / 2;
        current = ( deltaL - deltaR ) / 2;

        error = target - current;

        output = (error * kP) + (error - last) * kD;

        last = error;

        if(output > 0) {
          if(output > slewOutput + rate) {
            slewOutput += rate;
          } else {
            slewOutput = output;
          }
        }

        if(output < 0) {
          if(output < slewOutput - rate) {
            slewOutput -= rate;
          } else {
            slewOutput = output;
          }
        }

        if(slewOutput > speed) slewOutput = speed;
        if(slewOutput < -speed) slewOutput = speed;

        if(-tolerance < error < tolerance) {
          reset();
          withTolerance();
          withSlop();
          goto end;
        }

        left(slewOutput - slop());
        right(slewOutput + slop());
        break;
      }

      case 2: { // Turning
        deltaL = ( LF.get_position() + LB.get_position() ) / 2;
        deltaR = ( RF.get_position() + RB.get_position() ) / 2;
        current = ( deltaL + deltaR ) / 2;

        error = target - current;

        output = (error * kP) + (error - last) * kD;

        last = error;

        if(output > 0) {
          if(output > slewOutput + rate) {
            slewOutput += rate;
          } else {
            slewOutput = output;
          }
        }

        if(output < 0) {
          if(output < slewOutput - rate) {
            slewOutput -= rate;
          } else {
            slewOutput = output;
          }
        }

        if(slewOutput > speed) slewOutput = speed;
        if(slewOutput < -speed) slewOutput = speed;

        if(-tolerance < error < tolerance) {
          reset();
          withTolerance();
          withSlop();
          goto end;
        }

        left(slewOutput);
        right(-slewOutput);
        break;
      }

      case 3: { // Strafing
        deltaL = ( LF.get_position() - LB.get_position() ) / 2;
        deltaR = ( RB.get_position() - RF.get_position() ) / 2;
        current = ( deltaL + deltaR ) / 2;

        error = target - current;

        output = (error * kP) + (error - last) * kD;

        last = error;

        if(output > 0) {
          if(output > slewOutput + rate) {
            slewOutput += rate;
          } else {
            slewOutput = output;
          }
        }

        if(output < 0) {
          if(output < slewOutput - rate) {
            slewOutput -= rate;
          } else {
            slewOutput = output;
          }
        }

        if(slewOutput > speed) slewOutput = speed;
        if(slewOutput < -speed) slewOutput = speed;

        if(-tolerance < error < tolerance) {
          reset();
          withTolerance();
          withSlop();
          goto end;
        }

        LF.move_velocity(-slewOutput - slop(1));
        LB.move_velocity(slewOutput + slop(1));
        RF.move_velocity(-slewOutput - slop(1));
        RB.move_velocity(slewOutput + slop(1));
        break;
      }

      case 4: { // Aligning

      }

      default: { }
    }

    end:
    pros::delay(20);
  }
}

void Chassis::start(void* ignore) {
  if(!isRunning) {
    pros::delay(500);
    Chassis *that = static_cast<Chassis*>(ignore);
    that -> run();
  }
}

void Chassis::stop() {
  reset();
  isRunning = false;
}

void Chassis::left(int speed) {
  LF.move_velocity(speed);
  LB.move_velocity(speed);
}

void Chassis::right(int speed) {
  RF.move_velocity(-speed);
  RB.move_velocity(-speed);
}

double Chassis::slop(int mode) {
  switch(mode) {
    case 1: {
      return ((( LF.get_position() - LB.get_position() ) / 2) +
      (( RF.get_position() - RB.get_position() / 2)) + offset) * amp;
    }

    default: return ( deltaL + deltaR + offset) * amp; break;
  }
}
