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

  // Check if power button has been pressed
  Supervision::pwrButtonRead();

  // Check current system state
  if(currentMillis - prevSuperviseMillis > supervisionInterval){
    Supervision::supervise();   
    prevSuperviseMillis = currentMillis;
  }

  // Refresh display
  if(currentMillis - prevDisplayMillis > displayInterval){
    Screen::updateDisplay(&data);
    prevDisplayMillis = currentMillis;
  }

  // Send serial data
  if(currentMillis - prevSendMillis > sendInterval){
    //Serial.println("SEND_SERIAL");
    if(usb.active)
      usb.sendData();
    if(wls.active)
      wls.sendData();
    prevSendMillis = currentMillis;
  }

  // Check incoming serial buffer for commands
  if(usb.active)
    usb.checkSerialBuffer();
  if(wls.active){
    wls.checkSerialBuffer();
  }
  //Serial.println("MAIN_LOOP");
  //delay(1000);
}
