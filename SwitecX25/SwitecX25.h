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

#ifndef SwitecX25_h
#define SwitecX25_h
#include <Arduino.h>

class SwitecX25
{
 private:
   static const unsigned char pinCount = 4;
   static const unsigned char stateCount = 12;
   unsigned char pins[pinCount];
   unsigned char currentState;    // 12 steps 
   unsigned int currentStep;      // step we are currently at
   unsigned int targetStep;       // target we are moving to
   unsigned int steps;            // total steps available
   unsigned long time0;           // time when we entered this state
   unsigned int microDelay = 500;       // microsecs until next state
   static const unsigned int minimalMicroDelay = 500;       // <- dla zerowania
   boolean stopped;               // true if stopped
   
   void advance();
   void writeIO();
   void stepUp();
   void stepDown();
 public:
   
   SwitecX25(unsigned int steps, unsigned char pin1, unsigned char pin2, unsigned char pin3, unsigned char pin4);
  
   void zero();
   void update();
   void updateBlocking();
   void setPosition(unsigned int pos);
};


#endif