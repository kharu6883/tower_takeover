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

double Chassis::kP = 0.3, Chassis::kD = 0.3;

double Chassis::tolerance = 1, Chassis::amp = 0.2, Chassis::offset = 0;
std::vector<ChassisTarget> Chassis::target;
int Chassis::currentTarget = 0;
bool Chassis::isMultiTarget = false;
bool Chassis::isTurnToPoint = false;

double Chassis::thetaRel = 0;

double *Chassis::deltaL, *Chassis::deltaR, *Chassis::theta, *Chassis::posX, *Chassis::posY;

double Chassis::error = 0, Chassis::last = 0, Chassis::output = 0, Chassis::slewOutput = 0,
Chassis::errorX = 0, Chassis::errorY = 0, Chassis::lastX = 0, Chassis::lastY = 0, Chassis::turnError = 0, Chassis::lastTurn = 0,
Chassis::driveOutput = 0, Chassis::turnOutput = 0, Chassis::driveSlewOutput = 0, Chassis::turnSlewOutput = 0,
Chassis::nowTime = 0, Chassis::lastTime = 0, Chassis::elapsed = 0;

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

Chassis& Chassis::withTarget(Vector2 point, int speed, double rate) {
  target.push_back(ChassisTarget());
  target[target.size() - 1].x = point.x;
  target[target.size() - 1].y = point.y;
  target[target.size() - 1].speed = speed;
  target[target.size() - 1].rate = rate;
  return *this;
}

Chassis& Chassis::drive() {
  currentTarget = 0;
  isMultiTarget = true;
  isSettled = false;
  reset();
  mode = DRIVING;
  return *this;
}

Chassis& Chassis::drive(Vector2 point, int speed_, int rate_) {
  currentTarget = 0;
  if(target.size() != 1) target.resize(1);
  target[0].x = point.x;
  target[0].y = point.y;
  target[0].speed = speed_;
  target[0].rate = rate_;
  isSettled = false;
  reset();
  mode = DRIVING;
  return *this;
}

Chassis& Chassis::turn(double theta_, int speed_, int rate_) {
  currentTarget = 0;
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
  currentTarget = 0;
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
  currentTarget = 0;
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
  errorX = errorY = lastX = lastY = 0;
  driveOutput = driveSlewOutput = turnOutput = turnSlewOutput = 0;

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

  double angle;

  while(isRunning) {
    if(!pros::competition::is_autonomous()) goto end;

    thetaRel = abs( ( ( *theta / 360 ) - (int)*theta ) * 360 );

    switch(mode) {
      case DRIVING: { // Driving
        errorX = target[currentTarget].x - *posX;
        errorY = target[currentTarget].y - *posY;
        turnError = atan2( errorY, errorX ) * ( 180 / PI ) - *theta;

        driveOutput = sqrt( pow( errorX, 2 ) + pow( errorY, 2 ) ) * kP; /* + ( sqrt( pow( errorX, 2 ) + pow( errorY, 2 ) ) - sqrt( pow( lastX, 2 ) + pow( lastY, 2 ) ) ) * kD; */
        turnOutput = turnError * ( kP + 2 ) + ( turnError - lastTurn ) * kD;

        lastX = errorX;
        lastY = errorY;
        lastTurn = turnError;

        driveOutput = driveOutput / abs(turnOutput/5);

        if(driveOutput > 0) {
          if(driveOutput > driveSlewOutput + target[currentTarget].rate) {
            driveSlewOutput += target[currentTarget].rate;
          } else {
            driveSlewOutput = driveOutput;
          }
        } else if(driveOutput < 0) {
          if(driveOutput < driveSlewOutput - target[currentTarget].rate) {
            driveSlewOutput -= target[currentTarget].rate;
          } else {
            driveSlewOutput = driveOutput;
          }
        }

        if(turnOutput > 0) {
          if(turnOutput > turnSlewOutput + target[currentTarget].rate) {
            turnSlewOutput += target[currentTarget].rate;
          } else {
            turnSlewOutput = turnOutput;
          }
        } else if(turnOutput < 0) {
          if(turnOutput < turnSlewOutput - target[currentTarget].rate) {
            turnSlewOutput -= target[currentTarget].rate;
          } else {
            turnSlewOutput = turnOutput;
          }
        }

        if(errorX > -tolerance && errorX < tolerance && errorY > -tolerance && errorY < tolerance) {
          isSettled = true;
          isTurnToPoint = false;
          withConst().withTol().withSlop().reset();
          break;
        }

        left(driveSlewOutput - turnSlewOutput);
        right(driveSlewOutput + turnSlewOutput);
        break;
      }

      case TURNING: { // Turning
        if(!isTurnToPoint) {
          if(target[0].theta + *theta < *theta + 180) {
            error = target[0].theta - thetaRel;
          } else if(target[0].theta - *theta > *theta - 180) {
            error = target[0].theta - thetaRel;
          }
        } else {
          target[0].theta = atan2( target[0].y - *posY, target[0].x - *posX ) * ( 180 / PI );
          if(target[0].theta < 180) target[0].theta = target[0].theta;
          if(target[0].theta > 180) target[0].theta = 180 - target[0].theta;
          if(target[0].theta + *theta < *theta + 180) {
            error = target[0].theta - thetaRel;
          } else if(target[0].theta - *theta > *theta - 180) {
            error = target[0].theta - thetaRel;
          }
          error = (  - 180 ) - *theta;
        }

        output = ( error * ( kP + 2 ) ) + ( error - last ) * kD;

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
    case 0: return ( *deltaR + *deltaL + offset) * amp; break;
    case 1: return ( *deltaR - *deltaL ) * amp; break;

    default: return ( *deltaR + *deltaL + offset ) * amp; break;
  }
}
