/**
 * main.ino
 * *************************
 * For Kent State's ATR Lab  
 * 03/19/2021
 * Dan Maher
 * 
 * Code for operation of the custom 
 * power distribution board (PDB) PCB 
 * 
 * *******************************************************
 * 
 * *******************************************************
**/

#include "setup.h"

/*****************************/
/************MAIN*************/
/*****************************/
void loop() {
  unsigned long currentMillis = millis();
  Supervision::pwrButtonRead();

  if(currentMillis - prevSuperviseMillis > supervisionInterval){
    Supervision::supervise();   
    prevSuperviseMillis = currentMillis;
  }
  
  if(currentMillis - prevSendMillis > sendInterval){
    if(usb.active)
      usb.sendData();
    if(wls.active)
      wls.sendData();
    prevSendMillis = currentMillis;
  }

  if(usb.active)
    usb.checkSerialBuffer();
  if(wls.active){
    wls.checkSerialBuffer();
  }
}
