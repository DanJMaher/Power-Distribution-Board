#include"setup.h"

/******************************/
/*********PIN NAMES************/
/******************************/
// Power Switch Pins
const int pwrButtonPin = 28;
const int keepOnPin = 26;

// 11.1V Pins
const int monitorCtrlPin_11v = 22;
const int monitorPin_11v = A0;
const int relayPin_11v = 41;
const int ledPin_11v = 39;

// 22.2V pins
const int monitorCtrlPin_22v = 22;
const int monitorPin_22v = A0;
const int relayPin_22v = 41;
const int ledPin_22v = 39;

/******************************/
/**********GLOBALS*************/
/******************************/
// Max voltage inputs for mapping calibration
const float maxVolts_11v = 15.0;
const float maxVolts_22v = 25.0;
const float analogMax = 1023.0;

// Lower voltage limits before system automatically
// powers down
extern const float cuttoffVolts_11v = 10.0;
extern const float cuttoffVolts_22v = 17.0;

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
