/* 
Header file for all functions allowing abstraction of communications between 
the strategy-processor and the movement-processor
Those functions are only meant to be used by the strategy processor to give orders to the 
movement processor
 */
#ifndef COMMMVT_H
#define COMMMVT_H
#include "commFrame.h"

void moveXY(const float X, const float Y, commFrame &frame);
void moveRT(const float R, const float theta, commFrame &frame);
void servo(const float angle, const float servo, commFrame &frame);
void stop(commFrame &frame);

void speedConf(const float speed, commFrame &frame);
void accelConf(const float accel, commFrame &frame);

//Todo:
  //Strat to Movt functions
void requestCoords(); //Allows movt to send current robot's location to strat

  //Movt to Strat functions
void moveXYDone(); //Allows movt to inform strat that last movement is finished
void sendFrameError(); //movt reports frame error to strat
void ackFrame(); //basic acknowledge functions

#endif