#include "kari/control/chassis.h"
#include "kari/util/misc.h"

pros::Motor LF(1, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_COUNTS),
            LB(11, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_COUNTS),
            RF(10, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_COUNTS),
            RB(20, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_COUNTS);

pros::ADIGyro Gyro(7);

bool Chassis::isRunning = false,
Chassis::isSettled = true;
int Chassis::mode = IDLE;

double Chassis::kP_drive = 0.9, Chassis::kD_drive = 0.3, Chassis::kP_turn = 3.3, Chassis::kD_turn = 0.3;

double Chassis::tolerance = 1, Chassis::amp = 0.2, Chassis::offset = 0;
std::vector<ChassisTarget> Chassis::target;
int Chassis::currTarget = 0;
bool Chassis::isTurnToPoint = false;

double *Chassis::deltaL, *Chassis::deltaR, *Chassis::theta, *Chassis::posX, *Chassis::posY;

double Chassis::thetaRel = 0;
double Chassis::error = 0, Chassis::last = 0, Chassis::output = 0, Chassis::slewOutput = 0,
Chassis::driveError = 0, Chassis::driveLast = 0, Chassis::turnError = 0, Chassis::turnLast = 0,
Chassis::driveOutput = 0, Chassis::turnOutput = 0, Chassis::driveSlewOutput = 0, Chassis::turnSlewOutput = 0,
Chassis::totOutputL = 0, Chassis::totOutputR = 0;

Chassis::Chassis() { }

Chassis::Chassis(double * odomL_, double * odomR_, double * theta_, double * posX_, double * posY_) {
  deltaL = odomL_;
  deltaR = odomR_;
  theta = theta_;
  posX = posX_;
  posY = posY_;
}

Chassis::~Chassis() {
  reset();
}

Chassis& Chassis::calibrateGyro() {
  Gyro.reset();
  return *this;
}

Chassis& Chassis::withConst(double kPd, double kDd, double kPt, double kDt) {
  kP_drive = kPd;
  kD_drive = kDd;
  kP_turn = kPt;
  kD_turn = kDt;
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

Chassis& Chassis::withPoint(Vector2 point, int speed, double rate, bool reverse_) {
  target.push_back(ChassisTarget());
  target[target.size() - 1].x = point.x;
  target[target.size() - 1].y = point.y;
  target[target.size() - 1].speed = speed;
  target[target.size() - 1].rate = rate;
  target[target.size() - 1].reverse = reverse_;
  return *this;
}

Chassis& Chassis::withTarget(double target_, double theta_, int speed, double rate, bool reverse_) {
  target.push_back(ChassisTarget());
  target[target.size() - 1].x = target_;
  target[target.size() - 1].theta = theta_;
  target[target.size() - 1].speed = speed;
  target[target.size() - 1].rate = rate;
  target[target.size() - 1].reverse = reverse_;
  return *this;
}

Chassis& Chassis::drive() {
  currTarget = 0;
  isSettled = false;
  reset();
  mode = DRIVING;
  return *this;
}

Chassis& Chassis::drive(Vector2 point, int speed_, int rate_, bool reverse_) {
  currTarget = 0;
  if(target.size() != 1) target.resize(1);
  target[0].x = point.x;
  target[0].y = point.y;
  target[0].speed = speed_;
  target[0].rate = rate_;
  target[0].reverse = reverse_;
  isSettled = false;
  reset();
  mode = DRIVING;
  return *this;
}

Chassis& Chassis::turn(double theta_, int speed_, int rate_) {
  currTarget = 0;
  isTurnToPoint = false;
  if(target.size() != 1) target.resize(1);
  target[0].theta = theta_;
  target[0].speed = speed_;
  target[0].rate = rate_;
  isSettled = false;
  reset();
  mode = TURNING;
  return *this;
}

Chassis& Chassis::turn(Vector2 point, int speed_, int rate_) {
  currTarget = 0;
  isTurnToPoint = true;
  if(target.size() != 1) target.resize(1);
  target[0].x = point.x;
  target[0].y = point.y;
  target[0].speed = speed_;
  target[0].rate = rate_;
  isSettled = false;
  reset();
  mode = TURNING;
  return *this;
}

Chassis& Chassis::strafe(double target_, int speed_, int rate_) {
  currTarget = 0;
  if(target.size() != 1) target.resize(1);
  target[0].x = target_;
  target[0].speed = speed_;
  target[0].rate = rate_;
  isSettled = false;
  reset();
  mode = STRAFING;
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
  error = last = output = slewOutput = 0;
  driveError = driveLast = turnError = turnLast = 0;
  driveOutput = driveSlewOutput = turnOutput = turnSlewOutput = 0;
  totOutputL = totOutputR = 0;

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

int Chassis::getMode() {
  return mode;
}

void Chassis::setMode(int mode_) {
  mode = mode_;
}

void Chassis::clearArr() {
  target.clear();
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
        double currTheta = *theta;
        driveError = sqrt( pow( target[currTarget].x - *posX, 2) + pow( target[currTarget].y - *posY, 2) );
        driveOutput = driveError * kP_drive + ( driveError - driveLast ) * kD_drive;

        target[currTarget].theta = atan2( target[currTarget].y - *posY, target[currTarget].x - *posX ) * ( 180 / PI );

        if(target[currTarget].reverse) currTheta += 180;

        if( ( (int)( currTheta / 360 ) * 360 ) > target[currTarget].theta && ( (int)( currTheta / 360 ) * 360 ) < target[currTarget].theta + 180 ) {
          if(currTheta > 0) {
            thetaRel = floor((int)( currTheta / 360 )) * 360;
          } else {
            thetaRel = ceil((int)( currTheta / 360 )) * 360;
          }
        } else if( ( (int)( currTheta / 360 ) * 360 ) < target[currTarget].theta && ( (int)( currTheta / 360 ) * 360 ) > target[currTarget].theta + 180 ) {
          if(currTheta < 0) {
            thetaRel = ceil((int)( currTheta / 360 )) * 360;
          } else {
            thetaRel = floor((int)( currTheta / 360 )) * 360;
          }
        } else {
          if(currTheta > 0) {
            thetaRel = floor((int)( currTheta / 360 )) * 360;
          } else {
            thetaRel = ceil((int)( currTheta / 360 )) * 360;
          }
        }

        turnError = ( target[currTarget].theta + thetaRel ) - currTheta;
        if( abs(turnError) > abs( ( (target[currTarget].theta - 360) + thetaRel ) - currTheta) ) {
          turnError = ( (target[currTarget].theta - 360) + thetaRel ) - currTheta;
        }

        turnOutput = ( turnError * kP_turn ) + ( turnError - turnLast ) * kD_turn;

        driveLast = driveError;
        turnLast = turnError;

        if(target.size() - 1 == currTarget) {
          if(turnOutput > 0) {
            if(turnOutput > turnSlewOutput + target[currTarget].rate) {
              turnSlewOutput += target[currTarget].rate;
            } else {
              turnSlewOutput = turnOutput;
            }
          } else if(turnOutput < 0) {
            if(turnOutput < turnSlewOutput - target[currTarget].rate) {
              turnSlewOutput -= target[currTarget].rate;
            } else {
              turnSlewOutput = turnOutput;
            }
          }

          driveOutput /= abs(turnOutput / 5);

          if(driveOutput > 0) {
            if(driveOutput > driveSlewOutput + target[currTarget].rate) {
              driveSlewOutput += target[currTarget].rate;
            } else {
              driveSlewOutput = driveOutput;
            }
          } else if(driveOutput < 0) {
            if(driveOutput < driveSlewOutput - target[currTarget].rate) {
              driveSlewOutput -= target[currTarget].rate;
            } else {
              driveSlewOutput = driveOutput;
            }
          }
        } else {
          if(turnOutput > turnSlewOutput + target[currTarget].rate) {
            turnSlewOutput += target[currTarget].rate;
          } else if(turnOutput < turnSlewOutput - target[currTarget].rate) {
            turnSlewOutput -= target[currTarget].rate;
          } else {
            turnSlewOutput = turnOutput;
          }

          if(target[currTarget].speed > driveSlewOutput) driveSlewOutput += target[currTarget].rate;
          if(target[currTarget].speed < driveSlewOutput) driveSlewOutput -= target[currTarget].rate;

          driveSlewOutput /= abs(turnOutput / 50);
        }

        if(driveSlewOutput > target[currTarget].speed) driveSlewOutput = target[currTarget].speed;
        if(driveSlewOutput < -target[currTarget].speed) driveSlewOutput = -target[currTarget].speed;

        if(turnSlewOutput > target[currTarget].speed) turnSlewOutput = target[currTarget].speed;
        if(turnSlewOutput < -target[currTarget].speed) turnSlewOutput = -target[currTarget].speed;

        if(target[currTarget].reverse) {
          totOutputL = -turnSlewOutput - driveSlewOutput;
          totOutputR = turnSlewOutput - driveSlewOutput;
        } else {
          totOutputL = -turnSlewOutput + driveSlewOutput;
          totOutputR = turnSlewOutput + driveSlewOutput;
        }

        if(driveError < tolerance && driveError > -tolerance && turnError < tolerance && turnError > -tolerance) {
          if(target.size() - 1 == currTarget) {
            clearArr();
            isSettled = true;
            isTurnToPoint = false;
            withConst().withTol().withSlop().reset();
            break;
          } else {
            currTarget++;
            break;
          }
        }

        left(totOutputL);
        right(totOutputR);
        break;
      }

      case TURNING: { // Turning
        if(isTurnToPoint) {
          target[0].theta = atan2( target[0].y - *posY, target[0].x - *posX ) * ( 180 / PI );
        }

        if( ( (int)( *theta / 360 ) * 360 ) > target[0].theta && ( (int)( *theta / 360 ) * 360 ) < target[0].theta + 180 ) {
          if(*theta > 0) {
            thetaRel = floor((int)( *theta / 360 )) * 360;
          } else {
            thetaRel = ceil((int)( *theta / 360 )) * 360;
          }
        } else if( ( (int)( *theta / 360 ) * 360 ) < target[0].theta && ( (int)( *theta / 360 ) * 360 ) > target[0].theta + 180 ) {
          if(*theta < 0) {
            thetaRel = ceil((int)( *theta / 360 )) * 360;
          } else {
            thetaRel = floor((int)( *theta / 360 )) * 360;
          }
        }

        error = ( target[0].theta + thetaRel ) - *theta;
        if( abs(error) > abs( ( (target[0].theta - 360) + thetaRel ) - *theta) ) {
          error = ( (target[0].theta - 360) + thetaRel ) - *theta;
        }

        output = ( error * kP_turn ) + ( error - last ) * kD_turn;

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

        if(error > -tolerance && error < tolerance) {
          isSettled = true;
          isTurnToPoint = false;
          withConst().withTol().withSlop().reset();
          break;
        }

        left(-slewOutput);
        right(slewOutput);
        break;
      }

      case STRAFING: { // Strafing
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
  LF.move(speed);
  LB.move(speed);
}

void Chassis::right(int speed) {
  RF.move(-speed);
  RB.move(-speed);
}

double Chassis::slop(int mode) {
  switch(mode) {
    case 0: return ( *deltaR + *deltaL + offset) * amp; break; // Drive
    case 1: return ( *deltaR - *deltaL ) * amp; break; // Strafe

    default: return ( *deltaR + *deltaL + offset ) * amp; break;
  }
}
