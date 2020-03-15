#include "kari/util/odometry.h"
#include "kari/control/chassis.h"

pros::ADIEncoder LEncoder(5, 6, true),
                 REncoder(3, 4);

pros::Imu Imu_T(7), Imu_L(4), Imu_R(8);

bool Odom::isRunning = false;

int Odom::currentL = 0, Odom::currentR = 0;
int Odom::deltaL = 0, Odom::deltaR = 0, Odom::lastDeltaL = 0, Odom::lastDeltaR = 0;

double Odom::inertL = 0, Odom::inertR = 0, Odom::inertT = 0;
double Odom::thetaRad = 0, Odom::thetaDeg = 0, Odom::offset = 0, Odom::posX = 0, Odom::posY = 0;

double Odom::output = 0, Odom::Desiredtheta = 0, Odom::DesiredX = 0, Odom::DesiredY = 0;

int * Odom::getL() {
  return &currentL;
}

int * Odom::getR() {
  return &currentR;
}

int * Odom::getDL() {
  return &deltaL;
}

int * Odom::getDR() {
  return &deltaR;
}

double * Odom::getThetaRad() {
  return &thetaRad;
}

double * Odom::getThetaDeg() {
  return &thetaDeg;
}

double * Odom::getX() {
  return &posX;
}

double * Odom::getY() {
  return &posY;
}

Odom& Odom::calibrateGyro() {
  Imu_T.reset();
  Imu_L.reset();
  Imu_R.reset();

  while( Imu_T.is_calibrating() || Imu_L.is_calibrating() || Imu_R.is_calibrating() ) { pros::delay(20); }
  io::master.rumble(" . .");
  return *this;
}

Odom& Odom::zero() {
  float left = abs( Imu_L.get_heading() - 360 ) * PI / 180;
  float right = abs( Imu_R.get_heading() - 360 ) * PI / 180;

  float x = ( cos( left + PI ) + cos( right + PI ) ) / 2;
  float y = ( sin( left + PI ) + sin( right + PI ) ) / 2;

  offset = abs( atan2f(y, x) + PI );
  return *this;
}

Odom& Odom::reset() {
  posX = posY = 0;
  return *this;
}

void Odom::start(void *ignore) {
  if(!isRunning) {
    pros::delay(500);
    Odom *that = static_cast<Odom*>(ignore);
    that -> run();
  }
}

void Odom::run() {
  isRunning = true;

  while(isRunning) {
    inertL = abs( Imu_L.get_heading() - 360 ) * PI / 180;
    inertR = abs( Imu_R.get_heading() - 360 ) * PI / 180;

    float x = ( cos( inertL - offset + PI ) + cos( inertR - offset + PI ) ) / 2;
    float y = ( sin( inertL - offset + PI ) + sin( inertR - offset + PI ) ) / 2;

    thetaRad = abs( atan2f(y, x) + PI );
    thetaDeg = thetaRad * 180 / PI;

    currentL = LEncoder.get_value();
    currentR = REncoder.get_value();

    deltaL = currentL - lastDeltaL;
    deltaR = currentR - lastDeltaR;

    posX = posX + (( deltaL + deltaR ) / 2) * cos( thetaRad );
    posY = posY + (( deltaL + deltaR ) / 2) * sin( thetaRad );

    lastDeltaL = LEncoder.get_value();
    lastDeltaR = REncoder.get_value();

    pros::delay(10);
  }
}

void Odom::stop() {
  isRunning = false;
}
