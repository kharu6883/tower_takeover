#include "controller/chassis.h"

pros::Motor LF(LFPORT, MOTOR_GEARSET_6, 0, MOTOR_ENCODER_ROTATIONS),
LB(LBPORT, MOTOR_GEARSET_6, 0, MOTOR_ENCODER_ROTATIONS),
RF(RFPORT, MOTOR_GEARSET_6, 0, MOTOR_ENCODER_ROTATIONS),
RB(RBPORT, MOTOR_GEARSET_6, 0, MOTOR_ENCODER_ROTATIONS);

pros::ADIUltrasonic LSonic(SONIC_L_PING, SONIC_L_ECHO);
pros::ADIUltrasonic RSonic(SONIC_R_PING, SONIC_R_ECHO);
pros::ADIGyro Gyro(GYRO);

bool Chassis::isRunning = false,
Chassis::isSettled = true,
Chassis::usingGyro = false;

int Chassis::mode = IDLE;

double Chassis::kP = 40, Chassis::kD = 5;

double Chassis::tolerance = 2, Chassis::amp = 100, Chassis::offset = 0, Chassis::target = 0;
int Chassis::speed = 0, Chassis::rate = 4;

double Chassis::angle = 0, Chassis::gyroAmp = 2;

double Chassis::deltaL = 0, Chassis::deltaR = 0,
Chassis::current = 0, Chassis::last = 0, Chassis::error = 0, Chassis::derivative = 0,
Chassis::output = 0, Chassis::slewOutput = 0, Chassis::lastL = 0, Chassis::lastR = 0,
Chassis::outputL = 0, Chassis::outputR = 0, Chassis::nowTime = 0,
Chassis::lastTime = 0, Chassis::elapsed = 0;

Chassis::Chassis() { }

Chassis::~Chassis() {
  reset();
}

Chassis& Chassis::calibrateGyro() {
  Gyro.reset();
  return *this;
}

Chassis& Chassis::withTol(double tolerance_) {
  tolerance = tolerance_;
  return *this;
}

Chassis& Chassis::withSlop(double amp_, double offset_) {
  amp = amp_;
  offset = offset_;
  return *this;
}

Chassis& Chassis::withGyro(double angle_, double gyroAmp_) {
  angle = angle_;
  gyroAmp = gyroAmp_;
  usingGyro = true;
  return *this;
}

Chassis& Chassis::drive(double target_, int speed_, int rate_) {
  target = target_;
  speed = speed_;
  rate = rate_;
  isSettled = false;
  reset();
  mode = DRIVING;
  return *this;
}

Chassis& Chassis::turn(double target_, int speed_, int rate_) {
  target = target_;
  speed = speed_;
  rate = rate_;
  isSettled = false;
  reset();
  mode = TURNING;
  return *this;
}

Chassis& Chassis::align(double target_) {
  target = target_;
  isSettled = false;
  reset();
  mode = ALIGNING;
  return *this;
}

void Chassis::waitUntilSettled() {
  while(!isSettled) pros::delay(20);
}

void Chassis::reset() {
  current = last = error = derivative = output = slewOutput = 0;
  deltaL = deltaR = lastL = lastR = outputL = outputR = 0;

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

int Chassis::getGyro() {
  return Gyro.get_value();
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
    if(!pros::competition::is_autonomous()) goto end;

    switch(mode) {
      case DRIVING: { // Driving
        deltaL = LF.get_position();
        deltaR = RF.get_position();
        current = ( deltaR - deltaL ) / 2;

        error = target - current;

        output = (error * kP) + (error - last) * kD;

        last = error;

        if(output > 0) {
          if(output > slewOutput + rate) {
            slewOutput += rate;
          } else {
            slewOutput = output;
          }
        } else if(output < 0) {
          if(output < slewOutput - rate) {
            slewOutput -= rate;
          } else {
            slewOutput = output;
          }
        }

        if(slewOutput > speed) slewOutput = speed;
        if(slewOutput < -speed) slewOutput = -speed;

        if(output > -tolerance && output < tolerance) {
          isSettled = true;
          withTol().withSlop().reset();
          break;
        }

        if(!usingGyro) {
          left(slewOutput + slop());
          right(slewOutput - slop());
        } else {
          left(slewOutput - (((Gyro.get_value() / 5) + angle) * 2 * gyroAmp));
          right(slewOutput + (((Gyro.get_value() / 5) + angle) * 2 * gyroAmp));
        }

        break;
      }

      case TURNING: { // Turning
        deltaL = LF.get_position();
        deltaR = RF.get_position();
        current = -1 * ( deltaR + deltaL ) / 2;

        error = target - current;

        output = (error * (kP + 60)) + (error - last) * kD;

        last = error;

        if(output > 0) {
          if(output > slewOutput + rate) {
            slewOutput += rate;
          } else {
            slewOutput = output;
          }
        } else if(output < 0) {
          if(output < slewOutput - rate) {
            slewOutput -= rate;
          } else {
            slewOutput = output;
          }
        }

        if(slewOutput > speed) slewOutput = speed;
        if(slewOutput < -speed) slewOutput = -speed;

        if(output > -tolerance && output < tolerance) {
          isSettled = true;
          withTol().withSlop().reset();
          break;
        }

        left(slewOutput);
        right(-slewOutput);
        break;
      }

      case ALIGNING: { // Aligning
        deltaL = target - LSonic.get_value();
        deltaR = target - RSonic.get_value();

        outputL = deltaL * ( kP - 30 ) + ( deltaL - lastL ) * kD;
        outputR = deltaR * ( kP - 30 ) + ( deltaR - lastR ) * kD;

        lastL = deltaL;
        lastR = deltaR;

        if(deltaL > -tolerance && deltaL < tolerance && deltaR > -tolerance && deltaR < tolerance) {
          isSettled = true;
          withTol().withSlop().reset();
          break;
        }

        left(outputL);
        right(outputR);
        break;
      }

      default: {
        break;
      }
    }

    #ifdef DEBUG
    std::cout << "Left Front: " << LF.get_position() << ", Output: " << output << std::endl;
    #endif

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
  LF.move(-speed);
  LB.move(-speed);
}

void Chassis::right(int speed) {
  RF.move(speed);
  RB.move(speed);
}

double Chassis::slop(int mode) {
  switch(mode) {
    case 0: return ( deltaR + deltaL + offset) * amp; break;
    case 1: return ( deltaR - deltaL ) * amp; break;

    default: return ( deltaR + deltaL + offset ) * amp; break;
  }
}
