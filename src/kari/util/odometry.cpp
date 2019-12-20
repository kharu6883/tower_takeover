#include "kari/util/odometry.h"
#include "kari/control/chassis.h"

pros::ADIEncoder LEncoder(5, 6, true),
                 REncoder(3, 4);

bool Odometry::isRunning = false;

double Odometry::deltaL = 0, Odometry::deltaR = 0, Odometry::lastDeltaL = 0, Odometry::lastDeltaR = 0;

double Odometry::thetaRad = 0, Odometry::thetaDeg = 0, Odometry::posX = 0, Odometry::posY = 0;

double Odometry::amp = 1;

double Odometry::output = 0, Odometry::Desiredtheta = 0, Odometry::DesiredX = 0, Odometry::DesiredY = 0;

double errorx,errory;

int LslewOutput, RslewOutput,Loutput, Routput, turnOutput;

int velocity, kp;

double thetaerror;

double * Odometry::getL() {
  return &deltaL;
}

double * Odometry::getR() {
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
    deltaL = LEncoder.get_value() - lastDeltaL;
    deltaR = REncoder.get_value() - lastDeltaR;

    thetaRad = thetaRad + (( deltaL - deltaR ) / 2) / 7.5 / 23.34;
    thetaDeg = thetaRad * 180 / 3.14159265358979323846264338327950288419716939937;

    // posX = posX + ( cos(theta) * (( deltaL + deltaR ) / 2 )) * amp;
    // posY = posY + ( sin(theta) * (( deltaL + deltaR ) / 2 )) * amp;

    posX = posX + (( deltaL + deltaR ) / 2) * cos(thetaRad);
    posY = posY + (( deltaL + deltaR ) / 2) * sin(thetaRad);

    // std::cout << "Left: " << deltaL << ", Right: " << deltaR << ", Theta: " << theta << ", X: " << posX << ", Y: " << posY << std::endl;

    lastDeltaL = LEncoder.get_value();
    lastDeltaR = REncoder.get_value();

    pros::delay(10);
  }
}

void Odometry::stop() {
  isRunning = false;
}

void Odometry::point(double DesiredX, double DesiredY, double rate, double speed) {
  while(true)
  {
  //math angle
   errory=posY-DesiredY;
   errorx=posX-DesiredX;
   Desiredtheta=tan(errorx/errory);
   thetaerror=thetaRad-Desiredtheta;
   turnOutput=thetaerror*2;
  //math     velocity

   velocity=(sqrt(pow(errorx, 2)+pow(errory, 2)))*0.1;
   if(velocity>speed)velocity=speed;
   if(velocity<-speed)velocity=-speed;
  //output control
   Loutput=turnOutput+(velocity/thetaerror);
   Routput=-turnOutput+(velocity/thetaerror);

   if(Loutput > 0) {
     if(Loutput > LslewOutput + rate) {
       LslewOutput += rate;
     } else {
       LslewOutput = Loutput;
     }
   } else if(output < 0) {
     if(Loutput < LslewOutput - rate) {
       LslewOutput -= rate;
     } else {
       LslewOutput = Loutput;
     }
   }

   if(Routput > 0) {
     if(Routput > RslewOutput + rate) {
       RslewOutput += rate;
     } else {
       RslewOutput = Routput;
     }
   } else if(Routput < 0) {
     if(Routput < RslewOutput - rate) {
       RslewOutput -= rate;
     } else {
       RslewOutput = Routput;
     }
   }

   LF.move(LslewOutput);
   LB.move(LslewOutput);
   RF.move(RslewOutput);
   RB.move(RslewOutput);
   pros::delay(10);
  }
}
