/**
 * setup.h
 * *************************
 * For Kent State's ATR Lab  
 * 03/19/2021
 * Dan Maher
 * 
 * Code for operation of revision 1 of the custom 
 * power distribution board (PDB) PCB 
 * 
 * *******************************************************
 * Pin numbers and globals declarations
 * *******************************************************
**/

#ifndef SETUP_H
#define SETUP_H

/******************************/
/**********HEADERS*************/
/******************************/
// Won't compile without these forward class declarations
class Comm;
class Data;

// Screen
#include <Adafruit_SSD1306.h>

#include <ArduinoJson.h>
#include "supervision.h"
#include "data.h"
#include "comm.h"
#include "screen.h"

/******************************/
/*********PIN NAMES************/
/******************************/
// Power Switch Pins
extern const int pwrButtonPin;
extern const int keepOnPin;

// 11.1V Pins
extern const int monitorCtrlPin_11v;
extern const int monitorPin_11v;
extern const int relayPin_11v;
extern const int ledPin_11v;

// 22.2V pins
extern const int monitorCtrlPin_22v;
extern const int monitorPin_22v;
extern const int relayPin_22v;
extern const int ledPin_22v;

// Menu navigation pins
extern const int upBtn;
extern const int downBtn;
extern const int leftBtn;
extern const int rightBtn;
extern const int centerBtn;

/******************************/
/**********GLOBALS*************/
/******************************/
// Max voltage inputs for mapping calibration
extern const float maxVolts_11v;
extern const float maxVolts_22v;
extern const float analogMax;

// Lower voltage limits before system automatically
// powers down
extern const float cuttoffVolts_11v;
extern const float cuttoffVolts_22v;

// Time tracking
extern unsigned long prevSendMillis;
extern unsigned long prevSuperviseMillis;
extern unsigned long prevDisplayMillis;
// Time between sending json doc via serial in milliseconds
extern const int sendInterval;
// Time between checking system state in milliseconds
extern const int supervisionInterval;
// Time between updating the I2C display
extern const int displayInterval;


// Data object to store system status elements
extern Data data;

// Screen object
extern Adafruit_SSD1306 display;

// Objects for storing comm path information and sending messages
extern Comm usb;
extern Comm wls;

#endif
