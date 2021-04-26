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
  data.setVoltage_11(voltage);
  if(voltage <= cutoffVolts_11v){
    char *shutdownCode = "auto";
    Supervision::powerDown(shutdownCode);                   //UNCOMMENT ONCE VOLTAGE SOURCES ARE IN PLACE
  }

  voltage = readVoltage_22v();
  data.setVoltage_22(voltage);
  if(voltage <= cutoffVolts_22v){
    char *shutdownCode = "auto";
    Supervision::powerDown(shutdownCode);                   //UNCOMMENT ONCE VOLTAGE SOURCES ARE IN PLACE
  }
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

static void Supervision::setRelay(int relay, bool condition){
  if(relay == relayPin_11v){                         // Maybe better to make a relay class to avoid this slop?
    digitalWrite(relay, condition);
    digitalWrite(ledPin_11v, condition);
    data.setRelayStatus_11(condition);
  }else if(relay == relayPin_22v){
    digitalWrite(relay, condition);
    digitalWrite(ledPin_22v, condition);
    data.setRelayStatus_22(condition);
  }
}

static void Supervision::powerDown(char *code){
  setRelay(relayPin_11v, false);
  setRelay(relayPin_22v, false);
  
  if(usb.active)
    usb.sendShutdown(code);         
  if(wls.active)
    wls.sendShutdown(code);                                                               
  digitalWrite(keepOnPin, LOW);
  Screen::showShutdown(code);
}
