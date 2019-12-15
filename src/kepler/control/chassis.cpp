#include "kepler/control/chassis.h"
#include "kepler/util/misc.h"

pros::Motor LF(LFPORT, MOTOR_GEARSET_6, 0, MOTOR_ENCODER_COUNTS),
LB(LBPORT, MOTOR_GEARSET_6, 0, MOTOR_ENCODER_COUNTS),
RF(RFPORT, MOTOR_GEARSET_6, 0, MOTOR_ENCODER_COUNTS),
RB(RBPORT, MOTOR_GEARSET_6, 0, MOTOR_ENCODER_COUNTS);

pros::ADIUltrasonic LSonic(SONIC_L_PING, SONIC_L_ECHO);
pros::ADIUltrasonic RSonic(SONIC_R_PING, SONIC_R_ECHO);
pros::ADIGyro Gyro(GYRO);

bool Chassis::isRunning = false,
Chassis::isSettled = true,
Chassis::usingGyro = false;

int Chassis::mode = IDLE;

double Chassis::kP = 0.3, Chassis::kD = 0.3;

double Chassis::tolerance = 6, Chassis::amp = 0.2, Chassis::offset = 0;
std::vector<macro::ChassisTarget> Chassis::target;
int Chassis::currentTarget = 0;
bool Chassis::isMultiTarget = false;

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

Chassis& Chassis::withConst(double kP_, double kD_) {
  kP = kP_;
  kD = kD_;
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
  if(target.size() != 1) target.resize(1);
  target[0].angle = angle_;
  target[0].gyroAmp = gyroAmp_;
  usingGyro = true;
  return *this;
}

Chassis& Chassis::withTarget(double target_, int speed_, double angle_, double gyroAmp_, double rate_) {
  target.push_back(macro::ChassisTarget());
  target[target.size() - 1].length = target_;
  target[target.size() - 1].speed = speed_;
  target[target.size() - 1].angle = angle_;
  target[target.size() - 1].gyroAmp = gyroAmp_;
  target[target.size() - 1].rate = rate_;
  return *this;
}

Chassis& Chassis::drive() {
  currentTarget = 0;
  isMultiTarget = true;
  usingGyro = true;
  isSettled = false;
  reset();
  mode = DRIVING;
  return *this;
}

Chassis& Chassis::drive(double target_, int speed_, int rate_) {
  currentTarget = 0;
  if(target.size() != 1) target.resize(1);
  target[0].length = target_;
  target[0].speed = speed_;
  target[0].rate = rate_;
  isSettled = false;
  reset();
  mode = DRIVING;
  return *this;
}

Chassis& Chassis::turn(double target_, int speed_, int rate_) {
  currentTarget = 0;
  if(target.size() != 1) target.resize(1);
  target[0].length = target_;
  target[0].speed = speed_;
  target[0].rate = rate_;
  isSettled = false;
  reset();
  mode = TURNING;
  return *this;
}

Chassis& Chassis::align(double target_) {
  currentTarget = 0;
  if(target.size() != 1) target.resize(1);
  target[0].length = target_;
  isSettled = false;
  reset();
  mode = ALIGNING;
  return *this;
}

void Chassis::waitUntilSettled() {
  while(!isSettled) pros::delay(20);
}

void Chassis::tarePos() {
  LF.tare_position();
  LB.tare_position();
  RF.tare_position();
  RB.tare_position();
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

void Chassis::start(void* ignore) {
  if(!isRunning) {
    pros::delay(500);
    Chassis *that = static_cast<Chassis*>(ignore);
    that -> run();
  }
}

void Chassis::run() {
  isRunning = true;

  while(isRunning) {
    if(!pros::competition::is_autonomous()) goto end;

    switch(mode) {
      case DRIVING: { // Driving
        std::cout << ": " << target.size() << ", currentTarget: " << currentTarget << ", Output: " << output << ", SlewOutput: " << slewOutput << ", yeet: " << target[currentTarget].rate << std::endl;
        deltaL = LF.get_position();
        deltaR = RF.get_position();
        current = ( deltaR - deltaL ) / 2;

        error = target[currentTarget].length - current;

        output = (error * kP) + (error - last) * kD;

        last = error;

        if(!isMultiTarget || currentTarget == target.size() - 1) { // Is it last target?
          if(output > 0) {
            if(output > slewOutput + target[currentTarget].rate) {
              slewOutput += target[currentTarget].rate;
            } else {
              slewOutput = output;
            }
          } else if(output < 0) {
            if(output < slewOutput - target[currentTarget].rate) {
              slewOutput -= target[currentTarget].rate;
            } else {
              slewOutput = output;
            }
          }
        } else {
          if(output > 0) {
            if(target[currentTarget].speed > slewOutput) slewOutput += target[currentTarget].rate;
            if(target[currentTarget].speed < slewOutput) slewOutput -= target[currentTarget].rate;
          }

          if(output < 0) {
            if(-target[currentTarget].speed > slewOutput) slewOutput += target[currentTarget].rate;
            if(-target[currentTarget].speed < slewOutput) slewOutput -= target[currentTarget].rate;
          }
        }

        if(slewOutput > target[currentTarget].speed) slewOutput = target[currentTarget].speed;
        if(slewOutput < -target[currentTarget].speed) slewOutput = -target[currentTarget].speed;

        if(output > -tolerance && output < tolerance) {
          if(target.size() > 1 && target.size() - 1 > currentTarget) {
            tarePos();
            currentTarget++;
            break;
          } else {
            isSettled = true;
            target.clear();
            withConst().withTol().withSlop().reset();
            break;
          }
        }

        if(!usingGyro) {
          left(slewOutput + slop());
          right(slewOutput - slop());
        } else {
          left(slewOutput - (((Gyro.get_value() / 10) + target[currentTarget].angle) * 2 * target[currentTarget].gyroAmp));
          right(slewOutput + (((Gyro.get_value() / 10) + target[currentTarget].angle) * 2 * target[currentTarget].gyroAmp));
        }

        break;
      }

      case TURNING: { // Turning
        deltaL = LF.get_position();
        deltaR = RF.get_position();
        current = -1 * ( deltaR + deltaL ) / 2;

        error = target[0].length - current;

        output = ( error * (kP+0.6) ) + ( error - last ) * kD;

        last = error;

        if(output > 0) {
          if(output > slewOutput + target[0].rate) {
            slewOutput += target[0].rate;
          } else {
            slewOutput = output;
          }
        } else if(output < 0) {
          if(output < slewOutput - target[0].rate) {
            slewOutput -= target[0].rate;
          } else {
            slewOutput = output;
          }
        }

        if(slewOutput > target[0].speed) slewOutput = target[0].speed;
        if(slewOutput < -target[0].speed) slewOutput = -target[0].speed;

        if(output > -tolerance && output < tolerance) {
          isSettled = true;
          withConst().withTol().withSlop().reset();
          break;
        }

        left(slewOutput);
        right(-slewOutput);
        break;
      }

      case ALIGNING: { // Aligning
        deltaL = target[0].length - LSonic.get_value();
        deltaR = target[0].length - RSonic.get_value();

        outputL = ( deltaL * kP ) + ( deltaL - lastL ) * kD;
        outputR = ( deltaR * kP ) + ( deltaR - lastR ) * kD;

        lastL = deltaL;
        lastR = deltaR;

        if(deltaL > -tolerance && deltaL < tolerance && deltaR > -tolerance && deltaR < tolerance) {
          isSettled = true;
          withConst().withTol().withSlop().reset();
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
