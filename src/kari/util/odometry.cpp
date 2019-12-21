#include "kari/util/odometry.h"
#include "kari/control/chassis.h"

pros::ADIEncoder LEncoder(5, 6, true),
                 REncoder(3, 4);

bool Odometry::isRunning = false;

double Odometry::currentL = 0, Odometry::currentR = 0;
double Odometry::deltaL = 0, Odometry::deltaR = 0, Odometry::lastDeltaL = 0, Odometry::lastDeltaR = 0;

double Odometry::thetaRad = 0, Odometry::thetaDeg = 0, Odometry::posX = 0, Odometry::posY = 0;

double Odometry::output = 0, Odometry::Desiredtheta = 0, Odometry::DesiredX = 0, Odometry::DesiredY = 0;

double errorx,errory;

double velocity, kpd = 0.2, kpt= 2;

int slewOutput, output, slewturnOutput , turnOutput, angle;

double thetaerror;

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

    thetaRad = thetaRad + (( deltaL - deltaR ) / 2) / 7.5 / 23.34;
    thetaDeg = thetaRad * 180 / 3.14159265358979323846264338327950288419716939937;

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

void Odometry::turn(double angle, double rate, double speed, double tol) {
  thetaerror=0;
  turnOutput=0;
  slewturnOutput=0;

  while(true)
  {
  thetaerror=thetaDeg-angle;
  turnOutput=thetaerror*kpt;

  if(turnOutput > 0) {
    if(turnOutput > slewturnOutput + rate) {
      slewturnOutput += rate;
    } else {
      slewturnOutput = turnOutput;
    }
  } else if(turnOutput < 0) {
    if(turnOutput < slewturnOutput - rate) {
      slewturnOutput -= rate;
    } else {
      slewturnOutput = turnOutput;
    }
  }


  LF.move(-slewturnOutput);
  LB.move(-slewturnOutput);
  RF.move(slewturnOutput);
  RB.move(slewturnOutput);
if(thetaerror<tol && -tol<thetaerror)break;

  pros::delay(10);
}
}

void Odometry::point(double DesiredX, double DesiredY, double rate, double speed, double tol) {
  while(true) {
  //math angle
   errory=posY-DesiredY;
   errorx=posX-DesiredX;
   Desiredtheta=atan2(DesiredY - posY, DesiredX - posX);
   Desiredtheta=Desiredtheta * (180 / PI);
   thetaerror=thetaDeg-Desiredtheta;
   turnOutput=thetaerror*kpt;

  //math     velocity

   velocity=(sqrt(pow(errorx, 2)+pow(errory, 2)))*kpd/(turnOutput/10);
   if(velocity>speed)velocity=speed;
   if(velocity<-speed)velocity=-speed;
  //output control
   output=velocity;

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

   if(turnOutput > 0) {
     if(turnOutput > slewturnOutput + rate) {
       slewturnOutput += rate;
     } else {
       slewturnOutput = turnOutput;
     }
   } else if(turnOutput < 0) {
     if(turnOutput < slewturnOutput - rate) {
       slewturnOutput -= rate;
     } else {
       slewturnOutput = turnOutput;
     }
   }


   // LF.move(slewOutput);
   // LB.move(slewOutput);
   // RF.move(slewOutput);
   // RB.move(slewOutput);

   // LF.move(slewOutput+slewturnOutput);
   // LB.move(slewOutput+slewturnOutput);
   // RF.move(-slewOutput-slewturnOutput);
   // RB.move(-slewOutput-slewturnOutput);

   LF.move(-slewturnOutput+slewOutput);
   LB.move(-slewturnOutput+slewOutput);
   RF.move(slewturnOutput+slewOutput);
   RB.move(slewturnOutput+slewOutput);


   if(errorx<tol && -tol<errorx && errory<tol && -tol<errory)break;

   pros::delay(10);
  }
}
