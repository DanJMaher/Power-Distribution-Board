/**
 * setup.cpp
 * *************************
 * For Kent State's ATR Lab  
 * 03/19/2021
 * Dan Maher
 * 
 * Code for operation of revision 1 of the custom 
 * power distribution board (PDB) PCB 
 * 
 * *******************************************************
 * Pin numbers, globals, and setup() definitions
 * *******************************************************
**/

/******************************/
/**********HEADERS*************/
/******************************/
#include"setup.h"

/******************************/
/*********PIN NAMES************/
/******************************/
// Power Switch Pins
const int pwrButtonPin = 32;
const int keepOnPin = 46;

// 11.1V Pins
const int monitorCtrlPin_11v = 39;
const int monitorPin_11v = A15;
const int relayPin_11v = 41;
const int ledPin_11v = 23;

// 22.2V pins
const int monitorCtrlPin_22v = 23;
const int monitorPin_22v = A1;
const int relayPin_22v = 6;
const int ledPin_22v = 23;

// Menu navigation pins
const int upBtn = 2;
const int downBtn = 3;
const int leftBtn = 4;
const int rightBtn = 5;
const int centerBtn = 7;


/******************************/
/**********GLOBALS*************/
/******************************/
// Max voltage inputs for mapping calibration
const float maxVolts_11v = 15;
const float maxVolts_22v = 25.0;
const float analogMax = 1023.0;

// Minimum voltage limits before system automatically
// powers down
extern const float cuttoffVolts_11v = 10.0;
extern const float cuttoffVolts_22v = 17.0;

// Time tracking
unsigned long prevSendMillis = 0;
unsigned long prevSuperviseMillis = 0;
unsigned long prevDisplayMillis = 0;
// Time between sending json doc via serial in milliseconds
const int sendInterval = 1000;
// Time between checking system state in milliseconds
const int supervisionInterval = 1000;
// Time between updating the I2C display
const int displayInterval = 1000;

// Data object to store system status elements
Data data;

// Screen object
Adafruit_SSD1306 display(128, 64, &Wire, -1);

/******************************/
/********COMMUNICATION*********/
/******************************/
Comm usb(     
  true,       // Activates USB communication
  3,          // Max number of failed comm attempts for this path
  &Serial     // Path
  );
Comm wls(    
  true,        // Activates wireless (Serial1) communication
  3,           // max number umber of failed comm attempts for this path
  &Serial1     // Path
  );

void setup() {
  Serial.begin(115200);   //USB serial comm
  Serial1.begin(9600);  //Wireles serial comm (TX1/RX1)

  pinMode(upBtn, INPUT_PULLUP);
  pinMode(downBtn, INPUT_PULLUP);
  pinMode(leftBtn, INPUT_PULLUP);
  pinMode(rightBtn, INPUT_PULLUP);
  pinMode(centerBtn, INPUT_PULLUP);
  
  pinMode(pwrButtonPin, INPUT_PULLUP);
  pinMode(keepOnPin, OUTPUT);
  digitalWrite(keepOnPin, HIGH);
  
  pinMode(monitorCtrlPin_11v, OUTPUT);
  pinMode(monitorPin_11v,INPUT);
  pinMode(relayPin_11v, OUTPUT);
  pinMode(ledPin_11v, OUTPUT);
  digitalWrite(monitorCtrlPin_11v, HIGH);

  pinMode(monitorCtrlPin_22v, OUTPUT);
  pinMode(monitorPin_22v,INPUT);
  pinMode(relayPin_22v, OUTPUT);
  pinMode(ledPin_22v, OUTPUT);
  digitalWrite(monitorCtrlPin_22v, HIGH);

  // DEFAULT RELAYS TO OFF
  digitalWrite(relayPin_11v, LOW);
  digitalWrite(relayPin_22v, LOW);

  //Screen::initializeDisplay();
  usb.requestTime();
}
