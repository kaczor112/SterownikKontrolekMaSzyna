/*
 *  SwitecX25 Arduino Library
 *  Guy Carpenter, Clearwater Software - 2012
 *
 *  Licensed under the BSD2 license, see license.txt for details.
 *
 *  All text above must be included in any redistribution.
 */

/*
 * Thank you from the bottom of my heart:
 * Guy Carpenter, Clearwater Software
 * For the program below.
 * 
 * I have introduced modifications to Switec X27 for MaSzyna 2023 pressure gauges.
 * 
 * Best Regards Pawel Kaczmarczyk
 **/

#include <Arduino.h>
#include "SwitecX25.h"
static byte stateMap[] = {0x1, 0x5, 0x4, 0x7, 0x6, 0x2, 0xE, 0xA, 0xB, 0x8, 0x9, 0xD};

SwitecX25::SwitecX25(unsigned int steps, unsigned char pin1, unsigned char pin2, unsigned char pin3, unsigned char pin4)
{
  this->currentState = 0;
  this->steps = steps;
  this->pins[0] = pin1;
  this->pins[1] = pin2;
  this->pins[2] = pin3;
  this->pins[3] = pin4;
  for (int i=0;i<pinCount;i++) {
    pinMode(pins[i], OUTPUT);
  }
  
  stopped = true;
  currentStep = 0;
  targetStep = 0;
}

void SwitecX25::writeIO()
{

  byte mask = stateMap[currentState];  
  for (int i=0;i<pinCount;i++) {
    digitalWrite(pins[i], mask & 0x1);
    mask >>= 1;
  }
}

void SwitecX25::stepUp()
{
  if (currentStep < steps) {
    currentStep++;
    currentState = (currentState + 1) % stateCount;
    writeIO();
  }
}

void SwitecX25::stepDown()
{ 
  if (currentStep > 0) {
    currentStep--;
    currentState = (currentState + stateCount - 1) % stateCount;
    writeIO();
  }
}

void SwitecX25::zero()
{
  currentStep = steps - 1;
  for (unsigned int i=0;i<steps;i++) {
    stepDown();
    delayMicroseconds(minimalMicroDelay);
  }
  currentStep = 0;
  targetStep = 0;
}

void SwitecX25::advance()
{
  // detect stopped state
  if (currentStep==targetStep){
    stopped = true;
    time0 = micros();
    return;
  }

  if (currentStep<targetStep) {
    stepUp();
  } else {
    stepDown();
  }

  int delta = currentStep<targetStep ? targetStep-currentStep : currentStep-targetStep;

  // if(delta < 10) microDelay = 10000;
  // else if(delta < 20) microDelay = 9000;
  // else if(delta < 30) microDelay = 8000;
  // else if(delta < 40) microDelay = 7000;
  // else if(delta < 50) microDelay = 6000;
  // else if(delta < 60) microDelay = 5000;
  // else if(delta < 70) microDelay = 4000;
  // else if(delta < 80) microDelay = 3000;
  // else if(delta < 90) microDelay = 2000;

  
  if(delta < 100) microDelay = ((100 - delta) * 100) + 1000;
  else microDelay = minimalMicroDelay; // 500

  time0 = micros();
}

void SwitecX25::setPosition(unsigned int pos)
{
  if(targetStep != pos){
    // pos is unsigned so don't need to check for <0
    if (pos >= steps) pos = steps-1;
    targetStep = pos;
    if (stopped) {
      // reset the timer to avoid possible time overflow giving spurious deltas
      stopped = false;
      time0 = micros();
    }
  }
}

void SwitecX25::update()
{
  if (!stopped) {
    unsigned long delta = micros() - time0;
    if (delta >= microDelay) {
      advance();
    }
  }
}


//This updateMethod is blocking, it will give you smoother movements, but your application will wait for it to finish
void SwitecX25::updateBlocking()
{
  while (!stopped) {
    unsigned long delta = micros() - time0;
    if (delta >= microDelay) {
      advance();
    }
  }
}
