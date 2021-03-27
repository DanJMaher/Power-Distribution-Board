#include "supervision.h"

static void Supervision::supervise(){
  // Records current voltage levels
  float voltage;
  voltage = readVoltage_11v();
  Comm::appendJson("voltage", 0, voltage);
  if(voltage <= cuttoffVolts_11v){
    //Supervision::powerDown();                   //UNCOMMENT ONCE VOLTAGE SOURCES ARE IN PLACE
  }
    
  voltage = readVoltage_22v();
  if(voltage <= cuttoffVolts_22v){
    //Supervision::powerDown();                   //UNCOMMENT ONCE VOLTAGE SOURCES ARE IN PLACE
  }
  Comm::appendJson("voltage", 1, voltage);

  // Record current relay status
  bool relayStatus;
  relayStatus = digitalRead(relayPin_11v);
  Comm::appendJson("relay", 0, relayStatus);
  relayStatus = digitalRead(relayPin_22v);
  Comm::appendJson("relay", 1, relayStatus);
}

static void Supervision::pwrButtonRead() {
  if(digitalRead(pwrButtonPin) == 0)
    Supervision::powerDown();
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

static void Supervision::powerDown(){
  digitalWrite(relayPin_11v, LOW);
  digitalWrite(relayPin_22v, LOW);

  Comm::sendJson(&Serial);
  digitalWrite(keepOnPin, LOW);
    
  while(1){
      //Gives time for button to be released and capacitor to
      //discharge, then Arduino will power down
  }
}
