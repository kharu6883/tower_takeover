#include "kari/util/odometry.h"
#include "kari/control/chassis.h"

pros::ADIEncoder LEncoder(5, 6, true),
                 REncoder(3, 4);

bool Odometry::isRunning = false;

double Odometry::currentL = 0, Odometry::currentR = 0;
double Odometry::deltaL = 0, Odometry::deltaR = 0, Odometry::lastDeltaL = 0, Odometry::lastDeltaR = 0;

double Odometry::thetaRad = 0, Odometry::thetaDeg = 0, Odometry::posX = 0, Odometry::posY = 0;

double Odometry::output = 0, Odometry::Desiredtheta = 0, Odometry::DesiredX = 0, Odometry::DesiredY = 0;

double * Odometry::getL() {
  return &currentL;
}

double * Odometry::getR() {
  return &currentR;
}

double * Odometry::getDL() {
  return &deltaL;
}

double * Odometry::getDR() {
  return &deltaR;
}

double * Odometry::getThetaRad() {
  return &thetaRad;
}

double * Odometry::getThetaDeg() {
  return &thetaDeg;
}

double * Odometry::getX() {
  return &posX;
}

double * Odometry::getY() {
  return &posY;
}

void Odometry::reset(int theta_) {
  posX = posY = 0;
  thetaRad = theta_ * ( PI * 180 );
}

void Odometry::start(void *ignore) {
  if(!isRunning) {
    pros::delay(500);
    Odometry *that = static_cast<Odometry*>(ignore);
    that -> run();
  }
}

void Odometry::run() {
  isRunning = true;

  while(isRunning) {
    currentL = LEncoder.get_value();
    currentR = REncoder.get_value();
    deltaL = currentL - lastDeltaL;
    deltaR = currentR - lastDeltaR;

    thetaRad = thetaRad + ((( deltaL - deltaR ) / 2) / 7.5 / 23.34) * -1;
    thetaDeg = thetaRad * ( 180 / PI );

    if(thetaDeg < 0) {
      thetaDeg = (-thetaDeg);
      thetaDeg = fmod(thetaDeg, 360.0);
      thetaDeg = (-thetaDeg);
    } else {
      thetaDeg = fmod(thetaDeg, 360.0);
    }

    posX = posX + (( deltaL + deltaR ) / 2) * cos( thetaRad );
    posY = posY + (( deltaL + deltaR ) / 2) * sin( thetaRad );

    lastDeltaL = LEncoder.get_value();
    lastDeltaR = REncoder.get_value();

    pros::delay(10);
  }
}

void Odometry::stop() {
  isRunning = false;
}
