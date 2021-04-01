/**
 * supervision.h
 * *************************
 * For Kent State's ATR Lab  
 * 03/19/2021
 * Dan Maher
 * 
 * Code for operation of revision 1 of the custom 
 * power distribution board (PDB) PCB 
 * 
 * *******************************************************
 * The Supervision class provides methods for checking the
 * current status of many aspects of the system. Based on
 * user-set parameters, it modifys the system accordingly.
 * It also sends data, as it reads it, to a Data object.
 * *******************************************************
**/

#ifndef SUPERVISION_H
#define SUPERVISION_H

/******************************/
/**********HEADERS*************/
/******************************/
#include "setup.h"

/********************************/
/**********DECLARATIONS**********/
/********************************/
class Supervision{
  public:
  // Reads voltage levels, relay status, ribbon cable status, and acts accordingly
  static void supervise();

  // Checks to see if power button is pressed. If so, cuts power to relay
  // coils and to itself
  static void pwrButtonRead();

  // Deenergizes relays, sends a final JSON message, and powers down
  static void powerDown(char *code);
  
  private:
  // Reads voltage pin and maps analog input to real voltage values
  static float readVoltage_11v();
  static float readVoltage_22v();
};
#endif
