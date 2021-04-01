/**
 * supervision.cpp
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

/******************************/
/**********HEADERS*************/
/******************************/
#include "supervision.h"
#include "data.h"

/********************************/
/***********DEFINITIONS**********/
/********************************/
static void Supervision::supervise(){
  // Records current voltage levels
  float voltage;
  voltage = readVoltage_11v();
  data.storeVoltage_11(voltage);
  if(voltage <= cuttoffVolts_11v){
    char *shutdownCode = "auto";
    //Supervision::powerDown(shutdownCode);                   //UNCOMMENT ONCE VOLTAGE SOURCES ARE IN PLACE
  }
    
  voltage = readVoltage_22v();
  data.storeVoltage_22(voltage);
  if(voltage <= cuttoffVolts_22v){
    char *shutdownCode = "auto";
    //Supervision::powerDown(shutdownCode);                 //UNCOMMENT ONCE VOLTAGE SOURCES ARE IN PLACE
  }

  // Record current relay status
  bool relayStatus;
  relayStatus = digitalRead(relayPin_11v);
  data.storeRelayStatus_11(relayStatus);
  relayStatus = digitalRead(relayPin_22v);
  data.storeRelayStatus_22(relayStatus);
}

static void Supervision::pwrButtonRead() {
  if(digitalRead(pwrButtonPin) == 0){
    char *shutdownCode = "button";
    Supervision::powerDown(shutdownCode);
  }
}

static float Supervision::readVoltage_11v(){
  digitalWrite(monitorCtrlPin_11v, HIGH);
  float input = analogRead(monitorPin_11v);
  digitalWrite(monitorCtrlPin_11v, LOW);
  return (input * maxVolts_11v) / analogMax;
}

static float Supervision::readVoltage_22v(){
  digitalWrite(monitorCtrlPin_22v, HIGH);
  float input = analogRead(monitorPin_22v);
  digitalWrite(monitorCtrlPin_22v, LOW);
  return (input * maxVolts_22v) / analogMax;
}

static void Supervision::powerDown(char *code){
  digitalWrite(relayPin_11v, LOW);
  digitalWrite(relayPin_22v, LOW);

  if(usb.active)
    usb.sendShutdown(code);         
  if(wls.active)
    wls.sendShutdown(code);                                                               
  digitalWrite(keepOnPin, LOW);
    
  while(1){
      //Gives time for button to be released and capacitor to
      //discharge, then Arduino will power down
  }
}
