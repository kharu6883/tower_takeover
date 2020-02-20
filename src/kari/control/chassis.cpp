#include "kari/control/chassis.h"
#include "kari/util/misc.h"

pros::Motor LF(14, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_COUNTS),
            LB(11, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_COUNTS),
            RF(19, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_COUNTS),
            RB(20, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_COUNTS);

pros::ADIUltrasonic Ultrasonic(7, 8);

bool Chassis::isRunning = false,
Chassis::isSettled = true;
int Chassis::mode = IDLE;

double Chassis::kP_drive = 0.9, Chassis::kI_drive = 0, Chassis::kD_drive = 0.3, Chassis::kP_turn = 2, Chassis::kI_turn = 0.3, Chassis::kD_turn = 3.3;

double Chassis::tolerance = 1, Chassis::amp = 0.2, Chassis::offset = 0;
std::vector<ChassisTarget> Chassis::target;
int Chassis::currTarget = 0;
bool Chassis::isUsingPoint = false;

double *Chassis::odomL, *Chassis::odomR, *Chassis::theta, *Chassis::posX, *Chassis::posY;

double Chassis::current = 0, Chassis::initL = 0, Chassis::initR = 0, Chassis::deltaL = 0, Chassis::deltaR = 0,
Chassis::driveError = 0, Chassis::driveIntegral = 0, Chassis::driveLast = 0, Chassis::turnError = 0, Chassis::turnIntegral = 0, Chassis::turnLast = 0,
Chassis::driveOutput = 0, Chassis::driveOutput2 = 0, Chassis::driveOutput3 = 0, Chassis::driveOutput4 = 0, Chassis::turnOutput = 0, Chassis::driveSlewOutput = 0, Chassis::driveSlewOutput2 = 0, Chassis::driveSlewOutput3 = 0, Chassis::driveSlewOutput4 = 0 , Chassis::turnSlewOutput = 0,
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

Chassis& Chassis::withGain(double kP, double kI, double kD, bool isTurn) {
  if(!isTurn) {
    kP_drive = kP;
    kI_drive = kI;
    kD_drive = kD;
  } else {
    kP_turn = kP;
    kI_turn = kI;
    kD_turn = kD;
  }

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

Chassis& Chassis::withAngle(double theta_, int speed_, double rate_) {
  currTarget = 0;
  if( target.size() != 1 ) target.resize(1);
  target[0].theta = theta_;
  target[0].speedTurn = speed_;
  target[0].rateTurn = rate_;
  return *this;
}

Chassis& Chassis::withPoint(Vector2 point, int speed, double rate, bool reverse_) {
  isUsingPoint = true;
  target.push_back(ChassisTarget());
  target[target.size() - 1].x = point.x;
  target[target.size() - 1].y = point.y;
  target[target.size() - 1].speedDrive = speed;
  target[target.size() - 1].speedTurn = speed;
  target[target.size() - 1].rateDrive = rate;
  target[target.size() - 1].rateTurn = rate;
  target[target.size() - 1].reverse = reverse_;
  return *this;
}

Chassis& Chassis::withTarget(double target_, double theta_, int speed, double rate, bool reverse_) {
  isUsingPoint = false;
  target.push_back(ChassisTarget());
  target[target.size() - 1].x = target_;
  target[target.size() - 1].theta = theta_;
  target[target.size() - 1].speedDrive = speed;
  target[target.size() - 1].speedTurn = speed;
  target[target.size() - 1].rateDrive = rate;
  target[target.size() - 1].rateTurn = rate;
  target[target.size() - 1].reverse = reverse_;
  return *this;
}

Chassis& Chassis::drive() {
  currTarget = 0;
  isSettled = false;
  reset();
  if(isUsingPoint) mode = DRIVING_POINT;
    else mode = DRIVING_DIST;

  return *this;
}

Chassis& Chassis::drive(Vector2 point, int speed_, int rate_, bool reverse_) {
  currTarget = 0;
  if(target.size() != 1) target.resize(1);
  target[0].x = point.x;
  target[0].y = point.y;
  target[0].speedDrive = speed_;
  target[0].rateDrive = rate_;
  target[0].reverse = reverse_;
  isSettled = false;
  reset();
  mode = DRIVING_POINT;
  return *this;
}

Chassis& Chassis::drive(double target_, int speed_, double rate_) {
  currTarget = 0;
  if(target.size() != 1) target.resize(1);
  initL = *odomL;
  initR = *odomR;
  target[0].x = target_;
  target[0].speedDrive = speed_;
  target[0].rateDrive = rate_;
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
  target[0].speedDrive = speed_;
  target[0].rateDrive = rate_;
  isSettled = false;
  reset();
  mode = DRIVING_ULTRASONIC;
  return *this;
}

Chassis& Chassis::turn(Vector2 point, int speed_, int rate_) {
  currTarget = 0;
  isUsingPoint = true;
  if(target.size() != 1) target.resize(1);
  target[0].x = point.x;
  target[0].y = point.y;
  target[0].speedTurn = speed_;
  target[0].rateTurn = rate_;
  isSettled = false;
  reset();
  mode = TURNING;
  return *this;
}

Chassis& Chassis::turn(double theta_, int speed_, int rate_) {
  currTarget = 0;
  isUsingPoint = false;
  if(target.size() != 1) target.resize(1);
  target[0].theta = theta_;
  target[0].speedTurn = speed_;
  target[0].rateTurn = rate_;
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
  target[0].speedDrive = speed_;
  target[0].rateDrive = rate_;
  isSettled = false;
  reset();
  LF.tare_position();
  LB.tare_position();
  RF.tare_position();
  RB.tare_position();
  mode = STRAFING;
  return *this;
}

Chassis& Chassis::smartstrafe(double direction_, double theta_, double drivespeed_, double turnspeed_, double rate_, double rate2_) {
  currTarget = 0;
  if(target.size() != 1) target.resize(1);
  initL = *odomL;
  initR = *odomR;
  target[0].theta = theta_;
  target[0].direction = direction_;
  target[0].speedDrive = drivespeed_;
  target[0].speedTurn = turnspeed_;
  target[0].rateDrive = rate_;
  target[0].rateTurn = rate2_;
  isSettled = false;
  reset();
  LF.tare_position();
  LB.tare_position();
  RF.tare_position();
  RB.tare_position();
  mode = STRAFING_SMART;
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

    switch(mode) {
      case DRIVING_POINT: { // Drive to Point
        driveError = sqrt( pow( target[currTarget].x - *posX, 2) + pow( target[currTarget].y - *posY, 2) );

        driveIntegral += driveError;
        if( driveIntegral > WINDUP_LIMIT ) driveIntegral = WINDUP_LIMIT;
         else if( driveIntegral < -WINDUP_LIMIT ) driveIntegral = -WINDUP_LIMIT;

        driveOutput = ( driveError * kP_drive ) + ( driveIntegral * kI_drive ) + ( driveError - driveLast ) * kD_drive;

        // Turn Component
        target[currTarget].theta = atan2( target[currTarget].y - *posY, target[currTarget].x - *posX ) * ( 180 / PI );

        turnError = ( target[currTarget].theta - *theta ) * PI / 180;
        turnError = atan2( sin( turnError ), cos( turnError ) );
        turnError = turnError * 180 / PI;

        turnIntegral += turnError;
        if( turnIntegral > WINDUP_LIMIT ) turnIntegral = WINDUP_LIMIT;
         else if( turnIntegral < -WINDUP_LIMIT ) turnIntegral = -WINDUP_LIMIT;

        turnOutput = ( turnError * kP_turn ) + ( turnIntegral * kI_turn ) + ( turnError - turnLast ) * kD_turn;

        driveLast = driveError;
        turnLast = turnError;

        if(target.size() - 1 == currTarget) {
          if(turnOutput > 0) {
            if(turnOutput > turnSlewOutput + target[currTarget].rateTurn) turnSlewOutput += target[currTarget].rateTurn;
              else turnSlewOutput = turnOutput;
          } else if(turnOutput < 0) {
            if(turnOutput < turnSlewOutput - target[currTarget].rateTurn) turnSlewOutput -= target[currTarget].rateTurn;
              else turnSlewOutput = turnOutput;
          }

          driveOutput /= abs(turnOutput / 5);

          if(driveOutput > 0) {
            if(driveOutput > driveSlewOutput + target[currTarget].rateDrive) driveSlewOutput += target[currTarget].rateDrive;
              else driveSlewOutput = driveOutput;
          } else if(driveOutput < 0) {
            if(driveOutput < driveSlewOutput - target[currTarget].rateDrive) driveSlewOutput -= target[currTarget].rateDrive;
              else driveSlewOutput = driveOutput;
          }
        } else {
          if(turnOutput > turnSlewOutput + target[currTarget].rateTurn) turnSlewOutput += target[currTarget].rateTurn;
            else if(turnOutput < turnSlewOutput - target[currTarget].rateTurn) turnSlewOutput -= target[currTarget].rateTurn;
            else turnSlewOutput = turnOutput;

          if(target[currTarget].speedDrive > driveSlewOutput) driveSlewOutput += target[currTarget].rateDrive;
          if(target[currTarget].speedDrive < driveSlewOutput) driveSlewOutput -= target[currTarget].rateDrive;

          driveSlewOutput /= abs(turnOutput / 25);
        }

        if(driveSlewOutput > target[currTarget].speedDrive) driveSlewOutput = target[currTarget].speedDrive;
        if(driveSlewOutput < -target[currTarget].speedDrive) driveSlewOutput = -target[currTarget].speedDrive;

        if(turnSlewOutput > target[currTarget].speedTurn) turnSlewOutput = target[currTarget].speedTurn;
        if(turnSlewOutput < -target[currTarget].speedTurn) turnSlewOutput = -target[currTarget].speedTurn;

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
            isUsingPoint = false;
            isSettled = true;
            withGain().withTol().withSlop().reset();
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

      case DRIVING_DIST: { // Drive to Distance
        deltaL = *odomL - initL;
        deltaR = *odomR - initR;

        driveError = target[currTarget].x - ( deltaL + deltaR ) / 2;
        driveOutput = driveError * kP_drive + ( driveError - driveLast ) * kD_drive;

        turnError = ( target[currTarget].theta - *theta ) * PI / 180;
        turnError = atan2( sin( turnError ), cos( turnError ) );
        turnError = turnError * 180 / PI;

        turnIntegral += turnError;
        if( turnIntegral > WINDUP_LIMIT ) turnIntegral = WINDUP_LIMIT;
         else if( turnIntegral < -WINDUP_LIMIT ) turnIntegral = -WINDUP_LIMIT;

        turnOutput = ( turnError * kP_turn ) + ( turnError - turnLast ) * kD_turn;

        driveLast = driveError;
        turnLast = turnError;

        if(target.size() - 1 == currTarget) {
          if(turnOutput > 0) {
            if(turnOutput > turnSlewOutput + target[currTarget].rateTurn) turnSlewOutput += target[currTarget].rateTurn;
              else turnSlewOutput = turnOutput;
          } else if(turnOutput < 0) {
            if(turnOutput < turnSlewOutput - target[currTarget].rateTurn) turnSlewOutput -= target[currTarget].rateTurn;
              else turnSlewOutput = turnOutput;
          }

          driveOutput /= abs(turnSlewOutput / 10);

          if(driveOutput > 0) {
            if(driveOutput > driveSlewOutput + target[currTarget].rateDrive) driveSlewOutput += target[currTarget].rateDrive;
              else driveSlewOutput = driveOutput;
          } else if(driveOutput < 0) {
            if(driveOutput < driveSlewOutput - target[currTarget].rateDrive) driveSlewOutput -= target[currTarget].rateDrive;
              else driveSlewOutput = driveOutput;
          }
        } else {
          if(turnOutput > turnSlewOutput + target[currTarget].rateTurn) turnSlewOutput += target[currTarget].rateTurn;
            else if(turnOutput < turnSlewOutput - target[currTarget].rateTurn) turnSlewOutput -= target[currTarget].rateTurn;
            else turnSlewOutput = turnOutput;

          if(target[currTarget].speedDrive > driveSlewOutput) driveSlewOutput += target[currTarget].rateDrive;
          if(target[currTarget].speedDrive < driveSlewOutput) driveSlewOutput -= target[currTarget].rateDrive;

          driveSlewOutput /= abs(turnOutput / 25);
        }

        if(driveSlewOutput > target[currTarget].speedDrive) driveSlewOutput = target[currTarget].speedDrive;
        if(driveSlewOutput < -target[currTarget].speedDrive) driveSlewOutput = -target[currTarget].speedDrive;

        if(turnSlewOutput > target[currTarget].speedTurn) turnSlewOutput = target[currTarget].speedTurn;
        if(turnSlewOutput < -target[currTarget].speedTurn) turnSlewOutput = -target[currTarget].speedTurn;

        if(driveError < tolerance && driveError > -tolerance && turnError < tolerance && turnError > -tolerance) {
          if(target.size() - 1 == currTarget) {
            clearArr();
            isUsingPoint = false;
            isSettled = true;
            withGain().withTol().withSlop().reset();
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

      case DRIVING_ULTRASONIC: { // Drive with Ultrasonic
        deltaL = *odomL - initL;
        deltaR = *odomR - initR;

        driveError = target[currTarget].x - Ultrasonic.get_value();
        driveOutput = driveError * kP_drive + ( driveError - driveLast ) * kD_drive;
        driveLast = driveError;

        if(target.size() - 1 == currTarget) {
          if(driveOutput > 0) {
            if(driveOutput > driveSlewOutput + target[currTarget].rateDrive) driveSlewOutput += target[currTarget].rateDrive;
              else driveSlewOutput = driveOutput;
          } else if(driveOutput < 0) {
            if(driveOutput < driveSlewOutput - target[currTarget].rateDrive) driveSlewOutput -= target[currTarget].rateDrive;
              else driveSlewOutput = driveOutput;
          }
        } else {

        }

        if(driveSlewOutput > target[currTarget].speedDrive) driveSlewOutput = target[currTarget].speedDrive;
        if(driveSlewOutput < -target[currTarget].speedDrive) driveSlewOutput = -target[currTarget].speedDrive;

        if(driveError < tolerance && driveError > -tolerance && turnError < tolerance && turnError > -tolerance) {
          if(target.size() - 1 == currTarget) {
            clearArr();
            isSettled = true;
            withGain().withTol().withSlop().reset();
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

      case TURNING: { // Turning
        if( isUsingPoint ) target[currTarget].theta = atan2( target[currTarget].y - *posY, target[currTarget].x - *posX ) * ( 180 / PI );

        turnError = ( target[currTarget].theta - *theta ) * PI / 180;
        turnError = atan2( sin( turnError ), cos( turnError ) );
        turnError = turnError * 180 / PI;

        turnIntegral += turnError;
        if( turnIntegral > WINDUP_LIMIT ) turnIntegral = WINDUP_LIMIT;
         else if( turnIntegral < -WINDUP_LIMIT ) turnIntegral = -WINDUP_LIMIT;

        turnOutput = ( turnError * kP_turn ) + ( turnIntegral * kI_turn ) + ( turnError - turnLast ) * kD_turn;

        turnLast = turnError;

        if(turnOutput > 0) {
          if(turnOutput > turnSlewOutput + target[0].rateTurn) turnSlewOutput += target[0].rateTurn;
            else turnSlewOutput = turnOutput;
        } else if(turnOutput < 0) {
          if(turnOutput < turnSlewOutput - target[0].rateTurn) turnSlewOutput -= target[0].rateTurn;
            else turnSlewOutput = turnOutput;
        }

        if(turnSlewOutput > target[0].speedTurn) turnSlewOutput = target[0].speedTurn;
        if(turnSlewOutput < -target[0].speedTurn) turnSlewOutput = -target[0].speedTurn;

        if(turnError > -tolerance && turnError < tolerance) {
          isSettled = true;
          isUsingPoint = false;
          withGain().withTol().withSlop().reset();
          break;
        }

        left(-turnSlewOutput);
        right(turnSlewOutput);

        break;
      }

      case STRAFING: { // Strafing
        deltaL = ( LF.get_position() - LB.get_position() ) / 2;
        deltaR = ( RF.get_position() - RB.get_position() ) / 2;

        driveError = target[currTarget].x - ( deltaL + deltaR ) / 2;
        driveOutput = driveError * kP_drive + ( driveError - driveLast ) * kD_drive;
        driveLast = driveError;

        if(driveOutput > 0) {
          if(driveOutput > driveSlewOutput + target[currTarget].rateDrive) driveSlewOutput += target[currTarget].rateDrive;
            else driveSlewOutput = driveOutput;
        } else if(driveOutput < 0) {
          if(driveOutput < driveSlewOutput - target[currTarget].rateDrive) driveSlewOutput -= target[currTarget].rateDrive;
            else driveSlewOutput = driveOutput;
        }

        if(driveSlewOutput > target[currTarget].speedDrive) driveSlewOutput = target[currTarget].speedDrive;
        if(driveSlewOutput < -target[currTarget].speedDrive) driveSlewOutput = -target[currTarget].speedDrive;

        if(driveError < tolerance && driveError > -tolerance && turnError < tolerance && turnError > -tolerance) {
          isSettled = true;
          withGain().withTol().withSlop().reset();
          break;
        }

        LF.move(driveSlewOutput - slop(1));
        LB.move(-driveSlewOutput - slop(1));
        RF.move(driveSlewOutput - slop(1));
        RB.move(-driveSlewOutput - slop(1));
        break;
      }

      case STRAFING_SMART: { // Strafing to angle to angle lol
        // Turning
        turnError = ( target[currTarget].theta - *theta ) * PI / 180;
        turnError = atan2( sin( turnError ), cos( turnError ) );
        turnError = turnError * 180 / PI;

        turnOutput = ( turnError * kP_turn ) + ( turnError - turnLast ) * kD_turn;

        turnLast = turnError;

        // Strafing
        turnError = ( ( current-target[0].direction ) * PI ) / 180;

        driveOutput = cos( turnError );
        driveOutput2 = cos( turnError );
        driveOutput3 = sin( turnError );
        driveOutput4 = sin( turnError );

        driveOutput = driveOutput * kP_drive * 150;
        driveOutput2 = driveOutput2 * kP_drive * 150;
        driveOutput3 = driveOutput3 * kP_drive * 150;
        driveOutput4 = driveOutput4 * kP_drive * 150;

        // Slew
        if(turnOutput > 0) {
          if(turnOutput > turnSlewOutput + target[0].rateTurn) turnSlewOutput += target[0].rateTurn;
            else turnSlewOutput = turnOutput;
        } else if(turnOutput < 0) {
          if(turnOutput < turnSlewOutput - target[0].rateTurn) turnSlewOutput -= target[0].rateTurn;
            else turnSlewOutput = turnOutput;
        }

        if(turnSlewOutput > target[0].speedTurn) turnSlewOutput = target[0].speedTurn;
        if(turnSlewOutput < -target[0].speedTurn) turnSlewOutput = -target[0].speedTurn;

        driveOutput/=abs(turnSlewOutput);
        driveOutput2/=abs(turnSlewOutput);
        driveOutput3/=abs(turnSlewOutput);
        driveOutput4/=abs(turnSlewOutput);

        // Drive Output 1
        if(driveOutput > 0) {
          if(driveOutput > driveSlewOutput + target[0].rateDrive) driveSlewOutput += target[0].rateDrive;
            else driveSlewOutput = driveOutput;
        } else if(driveOutput < 0) {
          if(driveOutput < driveSlewOutput - target[0].rateDrive) driveSlewOutput -= target[0].rateDrive;
            else driveSlewOutput = driveOutput;
        }

        if(driveSlewOutput > target[0].speedDrive) driveSlewOutput = target[0].speedDrive;
        if(driveSlewOutput < -target[0].speedDrive) driveSlewOutput = -target[0].speedDrive;

        // Drive Output 2
        if(driveOutput2 > 0) {
          if(driveOutput2 > driveSlewOutput2 + target[0].rateDrive) driveSlewOutput2 += target[0].rateDrive;
            else driveSlewOutput2 = driveOutput2;
        } else if(driveOutput2 < 0) {
          if(driveOutput2 < driveSlewOutput2 - target[0].rateDrive) driveSlewOutput2 -= target[0].rateDrive;
            else driveSlewOutput2 = driveOutput2;
        }

        if(driveSlewOutput2 > target[0].speedDrive) driveSlewOutput2 = target[0].speedDrive;
        if(driveSlewOutput2 < -target[0].speedDrive) driveSlewOutput2 = -target[0].speedDrive;

        // Drive Output 3
        if(driveOutput3 > 0) {
          if(driveOutput3 > driveSlewOutput3 + target[0].rateDrive) driveSlewOutput3 += target[0].rateDrive;
            else driveSlewOutput3 = driveOutput3;
        } else if(driveOutput3 < 0) {
          if(driveOutput3 < driveSlewOutput3 - target[0].rateDrive) driveSlewOutput3 -= target[0].rateDrive;
            else driveSlewOutput3 = driveOutput3;
        }

        if(driveSlewOutput3 > target[0].speedDrive) driveSlewOutput3 = target[0].speedDrive;
        if(driveSlewOutput3 < -target[0].speedDrive) driveSlewOutput3 = -target[0].speedDrive;

        // Drive Output 4
        if(driveOutput4 > 0) {
          if(driveOutput4 > driveSlewOutput4 + target[0].rateDrive) driveSlewOutput4 += target[0].rateDrive;
            else driveSlewOutput4 = driveOutput4;
        } else if(driveOutput4 < 0) {
          if(driveOutput4 < driveSlewOutput4 - target[0].rateDrive) driveSlewOutput4 -= target[0].rateDrive;
            else driveSlewOutput4 = driveOutput4;
        }

        if(driveSlewOutput4 > target[0].speedDrive) driveSlewOutput4 = target[0].speedDrive;
        if(driveSlewOutput4 < -target[0].speedDrive) driveSlewOutput4 = -target[0].speedDrive;

        LF.move(driveSlewOutput2-driveSlewOutput3+turnSlewOutput);
        LB.move(driveSlewOutput+driveSlewOutput4+turnSlewOutput);
        RF.move(-driveSlewOutput-driveSlewOutput4+turnSlewOutput);
        RB.move(-driveSlewOutput2+driveSlewOutput3+turnSlewOutput);
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
