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

/******************************/
/*********LIBRARIES************/
/******************************/
#include <ArduinoJson.h>

/******************************/
/*********PIN NAMES************/
/******************************/
// Power Switch Pins
#define pwrButtonPin 28
#define keepOnPin 26

// 11.1V Pins
#define monitorCtrlPin_11v 22  
#define monitorPin_11v A0
#define relayPin_11v 41
#define ledPin_11v 39

// 22.2V pins
#define monitorCtrlPin_22v 22  
#define monitorPin_22v A0
#define relayPin_22v 41
#define ledPin_22v 39

/******************************/
/**********GLOBALS*************/
/******************************/
// Max voltage inputs for mapping calibration
const float maxVolts_11v = 15.0;
const float maxVolts_22v = 25.0;
const float analogMax = 1023.0;

// Time tracking
int prevMillis = 0;
// Time between sending json doc via serial in milliseconds
int sendInterval = 1000;

// Allocate space for arduinoJson document object
StaticJsonDocument<256> doc;  //NEED TO GET MORE ACCURATE ALLOCATION SIZE
                              //https://arduinojson.org/v6/assistant/

void setup() {
  Serial.begin(9600);
  
  pinMode(pwrButtonPin, INPUT_PULLUP);
  pinMode(keepOnPin, OUTPUT);
  digitalWrite(keepOnPin, HIGH);
  
  pinMode(monitorCtrlPin_11v, OUTPUT);
  pinMode(monitorPin_11v,INPUT);
  pinMode(relayPin_11v, OUTPUT);
  pinMode(ledPin_11v, OUTPUT);
  digitalWrite(relayPin_11v, HIGH);

  pinMode(monitorCtrlPin_22v, OUTPUT);
  pinMode(monitorPin_22v,INPUT);
  pinMode(relayPin_22v, OUTPUT);
  pinMode(ledPin_22v, OUTPUT);
  digitalWrite(relayPin_22v, HIGH);
}

void loop() {
  pwrButtonRead();
  
  if(millis() - prevMillis > sendInterval){
    createJson();
    sendJson();
    prevMillis = millis();
  }
  
}

/** Checks to see if power button is pressed. If so, cuts power to relay
 *  coils and to itself */
void pwrButtonRead() {
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

/** Records values to the json document
 */
void createJson(){
  // Record current voltage
  doc["voltage"][0] = mapVoltage(monitorPin_11v, maxVolts_11v);
  doc["voltage"][1] = mapVoltage(monitorPin_22v, maxVolts_22v);

  // Record current relay status
  doc["relay"][0] = digitalRead(relayPin_11v);
  doc["relay"][1] = digitalRead(relayPin_22v);
}

/** Serializes and transmits json doc
 */
void sendJson(){
    serializeJson(doc, Serial);
    Serial.println();
}

/** Maps analog input to real voltage values
 */
float mapVoltage(int voltagePin, float outMax) {
  float input = analogRead(voltagePin);
  return (input * outMax) / analogMax;
}
