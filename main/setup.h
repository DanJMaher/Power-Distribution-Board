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

#include <ArduinoJson.h>
#include "supervision.h"
#include "data.h"
#include "comm.h"

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
// Time between sending json doc via serial in milliseconds
extern const int sendInterval;
// Time between checking system state in milliseconds
extern const int supervisionInterval;

// Data object to store system status elements
extern Data data;

// Objects for storing comm path information and sending messages
extern Comm usb;
extern Comm wls;

#endif
