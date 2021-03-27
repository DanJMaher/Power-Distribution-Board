#ifndef SUPERVISION_H
#define SUPERVISION_H

#include "setup.h"
#include "data.h"

class Supervision{
  public:
  // Reads voltage levels, relay status, ribbon cable status, and acts accordingly
  static void supervise();

  // Checks to see if power button is pressed. If so, cuts power to relay
  // coils and to itself
  static void pwrButtonRead();

  private:
  // Reads voltage pin and maps analog input to real voltage values
  static float readVoltage_11v();
  static float readVoltage_22v();

  // Deenergizes relays, sends a final JSON message, and powers down
  static void powerDown();
  
};
#endif
