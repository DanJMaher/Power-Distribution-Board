#include "supervision.h"

static void Supervision::pwrButtonRead() {
  if(digitalRead(pwrButtonPin) == 0){
    digitalWrite(relayPin_11v, LOW);
    digitalWrite(relayPin_22v, LOW);
    digitalWrite(keepOnPin, LOW);
    
    while(1){
      //Gives time for button to be released and capacitor to
      //discharge, then Arduino will power down
    }
  }
}

static float Supervision::readVoltage(int voltagePin, int ctrlPin, float outMax){
  digitalWrite(ctrlPin, HIGH);
  float input = analogRead(voltagePin);
  digitalWrite(ctrlPin, LOW);
  return (input * outMax) / analogMax;
}
