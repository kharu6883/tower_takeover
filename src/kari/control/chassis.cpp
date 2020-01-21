#include "kari/control/chassis.h"
#include "kari/util/misc.h"

pros::Motor LF(14, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_COUNTS),
            LB(11, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_COUNTS),
            RF(19, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_COUNTS),
            RB(20, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_COUNTS);

pros::Imu Imu_T(7), Imu_L(4), Imu_R(8);
pros::ADIUltrasonic Ultrasonic(7, 8);

bool Chassis::isRunning = false,
Chassis::isSettled = true;
int Chassis::mode = IDLE;

double Chassis::kP_drive = 0.9, Chassis::kD_drive = 0.3, Chassis::kP_turn = 3.3, Chassis::kD_turn = 0.3;

double Chassis::tolerance = 1, Chassis::amp = 0.2, Chassis::offset = 0;
std::vector<ChassisTarget> Chassis::target;
int Chassis::currTarget = 0;
bool Chassis::isUsingOdom = true, Chassis::isUsingGyro = false, Chassis::isTurnToPoint = false;

double *Chassis::odomL, *Chassis::odomR, *Chassis::theta, *Chassis::posX, *Chassis::posY;

double Chassis::current = 0, Chassis::gyroOffset = 0, Chassis::thetaRel = 0, Chassis::initL = 0, Chassis::initR = 0, Chassis::deltaL = 0, Chassis::deltaR = 0,
Chassis::driveError = 0, Chassis::driveLast = 0, Chassis::turnError = 0, Chassis::turnLast = 0,
Chassis::driveOutput = 0, Chassis::driveOutput2 = 0, Chassis::driveOutput3 = 0, Chassis::driveOutput4 = 0, Chassis::turnOutput = 0, Chassis::driveSlewOutput = 0, Chassis::driveSlewOutput2 = 0 , Chassis::turnSlewOutput = 0,
Chassis::totOutputL = 0, Chassis::totOutputR = 0;

Chassis::Chassis() { }

Chassis::Chassis(double * odomL_, double * odomR_, double * theta_, double * posX_, double * posY_) {
  odomL = odomL_;
  odomR = odomR_;
  theta = theta_;
  posX = posX_;
  posY = posY_;
}

Chassis::~Chassis() {
  reset();
}

Chassis& Chassis::calibrateGyro() {
  Imu_T.reset();
  Imu_L.reset();
  Imu_R.reset();

  while(Imu_T.is_calibrating() || Imu_L.is_calibrating() || Imu_R.is_calibrating()) pros::delay(20);
  return *this;
}

Chassis& Chassis::tareGyro() {
  gyroOffset = current;
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

Chassis& Chassis::withSlop(double offset_, double amp_) {
  amp = amp_;
  offset = offset_;
  return *this;
}

Chassis& Chassis::withoutOdom() {
  isUsingOdom = false;
  return *this;
}

Chassis& Chassis::withGyro(double theta_) {
  isUsingGyro = true;
  target[target.size() - 1].theta = theta_;
  return *this;
}

Chassis& Chassis::withPoint(Vector2 point, int speed, double rate, bool reverse_) {
  isUsingOdom = true;
  target.push_back(ChassisTarget());
  target[target.size() - 1].x = point.x;
  target[target.size() - 1].y = point.y;
  target[target.size() - 1].speed = speed;
  target[target.size() - 1].rate = rate;
  target[target.size() - 1].reverse = reverse_;
  return *this;
}

Chassis& Chassis::withTarget(double target_, double theta_, int speed, double rate, bool reverse_) {
  isUsingOdom = false;
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
  if(isUsingOdom) {
    mode = DRIVING_POINT;
  } else {
    mode = DRIVING_DIST;
  }
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
  mode = DRIVING_POINT;
  return *this;
}

Chassis& Chassis::drive(double target_, int speed_, int rate_) {
  currTarget = 0;
  if(target.size() != 1) target.resize(1);
  initL = *odomL;
  initR = *odomR;
  target[0].x = target_;
  target[0].speed = speed_;
  target[0].rate = rate_;
  isSettled = false;
  reset();
  mode = DRIVING_DIST;
  return *this;
}

Chassis& Chassis::driveUltrasonic(double target_, int speed_, int rate_) {
  currTarget = 0;
  if(target.size() != 1) target.resize(1);
  initL = *odomL;
  initR = *odomR;
  target[0].x = target_;
  target[0].speed = speed_;
  target[0].rate = rate_;
  isSettled = false;
  reset();
  mode = DRIVING_ULTRASONIC;
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

Chassis& Chassis::strafe(double target_, int speed_, int rate_) {
  currTarget = 0;
  if(target.size() != 1) target.resize(1);
  initL = *odomL;
  initR = *odomR;
  target[0].x = target_;
  target[0].speed = speed_;
  target[0].rate = rate_;
  isSettled = false;
  reset();
  LF.tare_position();
  LB.tare_position();
  RF.tare_position();
  RB.tare_position();
  mode = STRAFING;
  return *this;
}

Chassis& Chassis::smartstrafe(double direction_, double theta_, double drivespeed_, double turnspeed_, double rate_) {
  currTarget = 0;
  if(target.size() != 1) target.resize(1);
  initL = *odomL;
  initR = *odomR;
  target[0].theta = theta_;
  target[0].direction = direction_;
  target[0].drivespeed = drivespeed_;
  target[0].turnspeed = turnspeed_;
  target[0].rate = rate_;
  isSettled = false;
  reset();
  LF.tare_position();
  LB.tare_position();
  RF.tare_position();
  RB.tare_position();
  mode = SMART_STRAFE;
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

    current = ( Imu_L.get_vex_heading() + Imu_L.get_vex_heading() ) / 2 - gyroOffset;

    switch(mode) {
      case DRIVING_POINT: { // Driving
        driveError = sqrt( pow( target[currTarget].x - *posX, 2) + pow( target[currTarget].y - *posY, 2) );
        driveOutput = driveError * kP_drive + ( driveError - driveLast ) * kD_drive;

        target[currTarget].theta = atan2( target[currTarget].y - *posY, target[currTarget].x - *posX ) * ( 180 / PI );

        if(target[currTarget].reverse) target[currTarget].theta += 180;

        if( ( (int)( *theta / 360 ) * 360 ) > target[currTarget].theta && ( (int)( *theta / 360 ) * 360 ) < target[currTarget].theta + 180 ) {
          if(*theta > 0) {
            thetaRel = floor((int)( *theta / 360 )) * 360;
          } else {
            thetaRel = ceil((int)( *theta / 360 )) * 360;
          }
        } else if( ( (int)( *theta / 360 ) * 360 ) < target[currTarget].theta && ( (int)( *theta / 360 ) * 360 ) > target[currTarget].theta + 180 ) {
          if(*theta < 0) {
            thetaRel = ceil((int)( *theta / 360 )) * 360;
          } else {
            thetaRel = floor((int)( *theta / 360 )) * 360;
          }
        } else {
          if(*theta > 0) {
            thetaRel = floor((int)( *theta / 360 )) * 360;
          } else {
            thetaRel = ceil((int)( *theta / 360 )) * 360;
          }
        }

        turnError = ( target[currTarget].theta + thetaRel ) - *theta;
        if( abs(turnError) > abs( ( (target[currTarget].theta - 360) + thetaRel ) - *theta) ) {
          turnError = ( (target[currTarget].theta - 360) + thetaRel ) - *theta;
        }

        turnOutput = ( turnError * kP_turn ) + ( turnError - turnLast ) * kD_turn;

        driveLast = driveError;
        turnLast = turnError;

        if(target.size() - 1 == currTarget) {
          if(turnOutput > 0) {
            if(turnOutput > turnSlewOutput + target[currTarget].rate) turnSlewOutput += target[currTarget].rate;
              else turnSlewOutput = turnOutput;
          } else if(turnOutput < 0) {
            if(turnOutput < turnSlewOutput - target[currTarget].rate) turnSlewOutput -= target[currTarget].rate;
              else turnSlewOutput = turnOutput;
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

          driveSlewOutput /= abs(turnOutput / 25);
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

      case DRIVING_DIST: {
        deltaL = *odomL - initL;
        deltaR = *odomR - initR;

        driveError = target[currTarget].x - ( deltaL + deltaR ) / 2;
        driveOutput = driveError * kP_drive + ( driveError - driveLast ) * kD_drive;

        if(isUsingGyro) {
          if(target[currTarget].reverse) target[currTarget].theta += 180;

          if( ( (int)( current / 360 ) * 360 ) > target[currTarget].theta && ( (int)( current / 360 ) * 360 ) < target[currTarget].theta + 180 ) {
            if(current > 0) {
              thetaRel = floor((int)( current / 360 )) * 360;
            } else {
              thetaRel = ceil((int)( current / 360 )) * 360;
            }
          } else if( ( (int)( current / 360 ) * 360 ) < target[currTarget].theta && ( (int)( current / 360 ) * 360 ) > target[currTarget].theta + 180 ) {
            if(current < 0) {
              thetaRel = ceil((int)( current / 360 )) * 360;
            } else {
              thetaRel = floor((int)( current / 360 )) * 360;
            }
          } else {
            if(current > 0) {
              thetaRel = floor((int)( current / 360 )) * 360;
            } else {
              thetaRel = ceil((int)( current / 360 )) * 360;
            }
          }

          turnError = ( target[currTarget].theta + thetaRel ) - current;
          if( abs(turnError) > abs( ( (target[currTarget].theta - 360) + thetaRel ) - current) ) {
            turnError = ( (target[currTarget].theta - 360) + thetaRel ) - current;
          }
        }

        turnOutput = ( turnError * kP_turn ) + ( turnError - turnLast ) * kD_turn;

        driveLast = driveError;
        turnLast = turnError;

        if(target.size() - 1 == currTarget) {
          if(turnOutput > 0) {
            if(turnOutput > turnSlewOutput + target[currTarget].rate) turnSlewOutput += target[currTarget].rate;
              else turnSlewOutput = turnOutput;
          } else if(turnOutput < 0) {
            if(turnOutput < turnSlewOutput - target[currTarget].rate) turnSlewOutput -= target[currTarget].rate;
              else turnSlewOutput = turnOutput;
          }

          driveOutput /= abs(turnSlewOutput / 10);

          if(driveOutput > 0) {
            if(driveOutput > driveSlewOutput + target[currTarget].rate) driveSlewOutput += target[currTarget].rate;
              else driveSlewOutput = driveOutput;
          } else if(driveOutput < 0) {
            if(driveOutput < driveSlewOutput - target[currTarget].rate) driveSlewOutput -= target[currTarget].rate;
              else driveSlewOutput = driveOutput;
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

          driveSlewOutput /= abs(turnOutput / 25);
        }

        if(driveSlewOutput > target[currTarget].speed) driveSlewOutput = target[currTarget].speed;
        if(driveSlewOutput < -target[currTarget].speed) driveSlewOutput = -target[currTarget].speed;

        if(turnSlewOutput > target[currTarget].speed) turnSlewOutput = target[currTarget].speed;
        if(turnSlewOutput < -target[currTarget].speed) turnSlewOutput = -target[currTarget].speed;

        if(driveError < tolerance && driveError > -tolerance && turnError < tolerance && turnError > -tolerance) {
          if(target.size() - 1 == currTarget) {
            clearArr();
            isUsingOdom = true;
            isUsingGyro = false;
            isSettled = true;
            withConst().withTol().withSlop().reset();
            break;
          } else {
            currTarget++;
            break;
          }
        }

        if(isUsingGyro) {
          totOutputL = turnSlewOutput + driveSlewOutput;
          totOutputR = -turnSlewOutput + driveSlewOutput;
        } else {
          totOutputL = driveSlewOutput - slop();
          totOutputR = driveSlewOutput + slop();
        }

        left(totOutputL);
        right(totOutputR);
        break;
      }

      case TURNING: { // Turning
          if(isUsingOdom) {
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

            turnError = ( target[0].theta + thetaRel ) - *theta;
            if( abs(turnError) > abs( ( (target[0].theta - 360) + thetaRel ) - *theta) ) {
              turnError = ( (target[0].theta - 360) + thetaRel ) - *theta;
            }
          } else {
            if( ( (int)( current / 360 ) * 360 ) > target[0].theta && ( (int)( current / 360 ) * 360 ) < target[0].theta + 180 ) {
              if(current > 0) {
                thetaRel = floor((int)( current / 360 )) * 360;
              } else {
                thetaRel = ceil((int)( current / 360 )) * 360;
              }
            } else if( ( (int)( current / 360 ) * 360 ) < target[0].theta && ( (int)( current / 360 ) * 360 ) > target[0].theta + 180 ) {
              if(current < 0) {
                thetaRel = ceil((int)( current / 360 )) * 360;
              } else {
                thetaRel = floor((int)( current / 360 )) * 360;
              }
            }


            turnError = ( target[0].theta + thetaRel ) - current;
            if( abs(turnError) > abs( ( (target[0].theta - 360) + thetaRel ) - current) ) {
              turnError = ( (target[0].theta - 360) + thetaRel ) - current;
            }
          }

          turnOutput = ( turnError * kP_turn ) + ( turnError - turnLast ) * kD_turn;

          turnLast = turnError;

          if(turnOutput > 0) {
            if(turnOutput > turnSlewOutput + target[0].rate) turnSlewOutput += target[0].rate;
              else turnSlewOutput = turnOutput;
          } else if(turnOutput < 0) {
            if(turnOutput < turnSlewOutput - target[0].rate) turnSlewOutput -= target[0].rate;
              else turnSlewOutput = turnOutput;
          }

          if(turnSlewOutput > target[0].speed) turnSlewOutput = target[0].speed;
          if(turnSlewOutput < -target[0].speed) turnSlewOutput = -target[0].speed;

          if(turnError > -tolerance && turnError < tolerance) {
            isSettled = true;
            isUsingOdom = true;
            isTurnToPoint = false;
            withConst().withTol().withSlop().reset();
            break;
          }

          if(isUsingOdom) {
            left(-turnSlewOutput);
            right(turnSlewOutput);
          } else {
            left(turnSlewOutput);
            right(-turnSlewOutput);
          }

          break;
        }

      case STRAFING: { // Strafing
        deltaL = ( LF.get_position() - LB.get_position() ) / 2;
        deltaR = ( RF.get_position() - RB.get_position() ) / 2;

        driveError = target[currTarget].x - ( deltaL + deltaR ) / 2;
        driveOutput = driveError * kP_drive + ( driveError - driveLast ) * kD_drive;
        driveLast = driveError;

        if(driveOutput > 0) {
          if(driveOutput > driveSlewOutput + target[currTarget].rate) driveSlewOutput += target[currTarget].rate;
            else driveSlewOutput = driveOutput;
        } else if(driveOutput < 0) {
          if(driveOutput < driveSlewOutput - target[currTarget].rate) driveSlewOutput -= target[currTarget].rate;
            else driveSlewOutput = driveOutput;
        }

        if(driveSlewOutput > target[currTarget].speed) driveSlewOutput = target[currTarget].speed;
        if(driveSlewOutput < -target[currTarget].speed) driveSlewOutput = -target[currTarget].speed;

        if(driveError < tolerance && driveError > -tolerance && turnError < tolerance && turnError > -tolerance) {
          isSettled = true;
          withConst().withTol().withSlop().reset();
          break;
        }

        LF.move(driveSlewOutput - slop(1));
        LB.move(-driveSlewOutput - slop(1));
        RF.move(driveSlewOutput - slop(1));
        RB.move(-driveSlewOutput - slop(1));
        break;
      }

      case DRIVING_ULTRASONIC: {
        deltaL = *odomL - initL;
        deltaR = *odomR - initR;

        driveError = target[currTarget].x - Ultrasonic.get_value();
        driveOutput = driveError * kP_drive + ( driveError - driveLast ) * kD_drive;
        driveLast = driveError;

        if(target.size() - 1 == currTarget) {
          if(driveOutput > 0) {
            if(driveOutput > driveSlewOutput + target[currTarget].rate) driveSlewOutput += target[currTarget].rate;
              else driveSlewOutput = driveOutput;
          } else if(driveOutput < 0) {
            if(driveOutput < driveSlewOutput - target[currTarget].rate) driveSlewOutput -= target[currTarget].rate;
              else driveSlewOutput = driveOutput;
          }
        } else {

        }

        if(driveSlewOutput > target[currTarget].speed) driveSlewOutput = target[currTarget].speed;
        if(driveSlewOutput < -target[currTarget].speed) driveSlewOutput = -target[currTarget].speed;

        if(driveError < tolerance && driveError > -tolerance && turnError < tolerance && turnError > -tolerance) {
          if(target.size() - 1 == currTarget) {
            clearArr();
            isUsingOdom = true;
            isSettled = true;
            withConst().withTol().withSlop().reset();
            break;
          } else {
            currTarget++;
            break;
          }
        }

        left(driveSlewOutput - ( slop() * 5 ));
        right(driveSlewOutput + ( slop() * 5 ));
        break;

      }

      case SMART_STRAFE: {


// turn
        if( ( (int)( current / 360 ) * 360 ) > target[0].theta && ( (int)( current / 360 ) * 360 ) < target[0].theta + 180 ) {
          if(current > 0) {
            thetaRel = floor((int)( current / 360 )) * 360;
          } else {
            thetaRel = ceil((int)( current / 360 )) * 360;
          }
        } else if( ( (int)( current / 360 ) * 360 ) < target[0].theta && ( (int)( current / 360 ) * 360 ) > target[0].theta + 180 ) {
          if(current < 0) {
            thetaRel = ceil((int)( current / 360 )) * 360;
          } else {
            thetaRel = floor((int)( current / 360 )) * 360;
          }
        }


        turnError = ( target[0].theta + thetaRel ) - current;
        if( abs(turnError) > abs( ( (target[0].theta - 360) + thetaRel ) - current) ) {
          turnError = ( (target[0].theta - 360) + thetaRel ) - current;
        }
        turnOutput = ( turnError * kP_turn ) + ( turnError - turnLast ) * kD_turn;

        turnLast = turnError;

// strafe
 driveOutput = sin(((turnError-45)*PI)/180);
driveOutput2 = -sin(((turnError+45)*PI)/180);
  driveOutput = driveOutput * kP_drive;
  driveOutput2 = driveOutput2 * kP_drive;


//slew work

           if(turnOutput > 0) {
             if(turnOutput > turnSlewOutput + target[0].rate) turnSlewOutput += target[0].rate;
               else turnSlewOutput = turnOutput;
           } else if(turnOutput < 0) {
             if(turnOutput < turnSlewOutput - target[0].rate) turnSlewOutput -= target[0].rate;
               else turnSlewOutput = turnOutput;
           }

           if(turnSlewOutput > target[0].turnspeed) turnSlewOutput = target[0].speed;
           if(turnSlewOutput < -target[0].turnspeed) turnSlewOutput = -target[0].speed;





 if(driveOutput > 0) {
   if(driveOutput > driveSlewOutput + target[0].rate) driveSlewOutput += target[0].rate;
     else driveSlewOutput = driveOutput;
 } else if(driveOutput < 0) {
   if(driveOutput < driveSlewOutput - target[0].rate) driveSlewOutput -= target[0].rate;
     else driveSlewOutput = driveOutput;
 }

 if(driveSlewOutput > target[0].drivespeed) driveSlewOutput = target[0].speed;
 if(driveSlewOutput < -target[0].drivespeed) driveSlewOutput = -target[0].speed;




  if(driveOutput2 > 0) {
    if(driveOutput2 > driveSlewOutput2 + target[0].rate) driveSlewOutput2 += target[0].rate;
      else driveSlewOutput2 = driveOutput2;
  } else if(driveOutput2 < 0) {
    if(driveOutput2 < driveSlewOutput2 - target[0].rate) driveSlewOutput2 -= target[0].rate;
      else driveSlewOutput2 = driveOutput2;
  }

  if(driveSlewOutput2 > target[0].drivespeed) driveSlewOutput2 = target[0].speed;
  if(driveSlewOutput2 < -target[0].drivespeed) driveSlewOutput2 = -target[0].speed;



        LF.move(turnSlewOutput+driveSlewOutput2);
        LB.move(-turnSlewOutput+driveSlewOutput);
        RF.move(turnSlewOutput+driveSlewOutput);
        RB.move(-turnSlewOutput+driveSlewOutput2);

      }

      default: {
        break;
      }
    }

    #ifdef DEBUG
    std::cout << "Left Front: " << LF.get_position() << ", Output: " << output << std::endl;
    #endif

    std::cout << Imu_T.get_vex_heading() << " " << Imu_L.get_vex_heading() << " " << Imu_R.get_vex_heading() << std::endl;

    end:
    pros::delay(10);
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
    case 0: return ( deltaL - deltaR + offset) * amp; break; // Drive
    case 1: return ( ( *odomL - initL ) - ( *odomR - initR ) + offset ) * amp; break; // Strafe

    default: return ( deltaL - deltaR + offset ) * amp; break;
  }
}
