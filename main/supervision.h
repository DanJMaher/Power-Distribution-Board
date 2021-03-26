#ifndef SUPERVISION_H
#define SUPERVISION_H

#include "setup.h"

class Supervision{
  public:
  // Checks to see if power button is pressed. If so, cuts power to relay
  // coils and to itself
  static void pwrButtonRead();
  
  // Reads voltage pin and maps analog input to real voltage values
  static float readVoltage(int voltagePin, int ctrlPin, float outMax);
};
#endif
