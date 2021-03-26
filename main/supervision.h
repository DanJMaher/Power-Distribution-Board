#ifndef SUPERVISION_H
#define SUPERVISION_H

#include "setup.h"
#include "comm.h"

class Supervision{
  public:
  // Reads voltage levels, relay status, ribbon cable status, and acts accordingly
  static void supervise();
  
  // Reads voltage pin and maps analog input to real voltage values
  static float readVoltage_11v();
  static float readVoltage_22v();

  // Checks to see if power button is pressed. If so, cuts power to relay
  // coils and to itself
  static void pwrButtonRead();

  private:
  static void powerDown();
};
#endif
