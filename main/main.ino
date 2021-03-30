/**
 * main.ino
 * *************************
 * For Kent State's ATR Lab  
 * 03/19/2021
 * Dan Maher
 * 
 * Code for operation of revision 1 of the custom 
 * power distribution board (PDB) PCB 
 * 
 * *******************************************************
 * 
 * *******************************************************
**/

/******************************/
/**********HEADERS*************/
/******************************/
#include "setup.h"
#include "comm.h"

/*****************************/
/************MAIN*************/
/*****************************/
void loop() {
  Supervision::pwrButtonRead();
  
  if(millis() - prevMillis > sendInterval && !responseExpected){
    Supervision::supervise();
    Comm::sendData(&Serial);
    prevMillis = millis();
  }
  Comm::checkSerialBuffer(&Serial);
  if(serial1Active){
    Comm::checkSerialBuffer(&Serial1);
  }
}
