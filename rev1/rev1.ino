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
unsigned long prevMillis = 0;
// Time between sending json doc via serial in milliseconds
const int sendInterval = 1000;

// Allocates space for arduinoJson document object. Bool to track if a message has been sent. but a response has not yet occurred.
// Limit for system to track if comm fails are a problem.
StaticJsonDocument<256> transmitDoc;  //NEED TO GET MORE ACCURATE ALLOCATION SIZE https://arduinojson.org/v6/assistant/
StaticJsonDocument<256> receiveDoc;
bool responseExpected = false;
int commFailCount = 0;
const int commFailLimit = 3;

void setup() {
  Serial.begin(9600);
  
  pinMode(pwrButtonPin, INPUT_PULLUP);
  pinMode(keepOnPin, OUTPUT);
  digitalWrite(keepOnPin, HIGH);
  
  pinMode(monitorCtrlPin_11v, OUTPUT);
  pinMode(monitorPin_11v,INPUT);
  pinMode(relayPin_11v, OUTPUT);
  pinMode(ledPin_11v, OUTPUT);
  digitalWrite(monitorCtrlPin_11v, LOW);
  digitalWrite(relayPin_11v, HIGH);

  pinMode(monitorCtrlPin_22v, OUTPUT);
  pinMode(monitorPin_22v,INPUT);
  pinMode(relayPin_22v, OUTPUT);
  pinMode(ledPin_22v, OUTPUT);
  digitalWrite(relayPin_22v, HIGH);
}

void loop() {
  pwrButtonRead();

  
  if(millis() - prevMillis > sendInterval && !responseExpected){
    createJson();
    sendJson();
    prevMillis = millis();
  }
  checkSerialBuffer();
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
  transmitDoc["voltage"][0] = readVoltage(monitorPin_11v, monitorCtrlPin_11v, maxVolts_11v);
  transmitDoc["voltage"][1] = readVoltage(monitorPin_22v, monitorCtrlPin_22v, maxVolts_22v);

  // Record current relay status
  transmitDoc["relay"][0] = digitalRead(relayPin_11v);
  transmitDoc["relay"][1] = digitalRead(relayPin_22v);

  Serial.flush();
}

/** Serializes and transmits json doc
 */
void sendJson(){
    serializeJson(transmitDoc, Serial);
    Serial.println();    
    Serial.flush();
    responseExpected = true;
}

/** Checks the USB serial port for a command
 */
void checkSerialBuffer(){
  if(Serial.available() < 1)
    return;

  parseSerial();
}

void parseSerial(){
  DeserializationError err = deserializeJson(receiveDoc, Serial.readString());
  if(err) {
    //ADD ERROR HANDLING HERE
    return;
  }
  if(responseExpected)
    if(receiveDoc["alive"]){
      responseExpected = false;
      commFailCount = 0;
    }else if(commFailCount == commFailLimit){
      // COMM FAIL ACTION
    }else{
      commFailCount++;
    }
    
  // ADD REMOTE COMMAND HANDLING HERE
}


/** Maps analog input to real voltage values
 */
float readVoltage(int voltagePin, int ctrlPin, float outMax) {
  digitalWrite(ctrlPin, HIGH);
  float input = analogRead(voltagePin);
  digitalWrite(ctrlPin, LOW);
  return (input * outMax) / analogMax;
}