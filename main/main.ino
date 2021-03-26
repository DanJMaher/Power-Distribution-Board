/**
 * 
 * For Kent State's ATR Lab  
 * 03/19/2021
 * Dan Maher
 * 
 * Code for operation of revision 1 of the custom 
 * power distribution board (PDB) PCB 
 * 
**/

#include "setup.h"
#include "comm.h"

void loop() {
  Supervision::pwrButtonRead();
  
  if(millis() - prevMillis > sendInterval && !responseExpected){
    Supervision::supervise();
    Comm::sendJson(&Serial);
    prevMillis = millis();
  }
  Comm::checkSerialBuffer(&Serial);
}



//Check power button (persistant)
//check voltage & send json every n seconeds
