/**
 * data.cpp
 * *************************
 * For Kent State's ATR Lab  
 * 03/19/2021
 * Dan Maher
 * 
 * Code for operation of revision 1 of the custom 
 * power distribution board (PDB) PCB 
 * 
 * *******************************************************
 * The Data class provides a central storage location for
 * voltage, relay status, and time. The Comm class
 * generated a JSON document using data stored in a
 * Data object.
 * *******************************************************
**/

/******************************/
/**********HEADERS*************/
/******************************/
#include "data.h"

/********************************/
/***********DEFINITIONS**********/
/********************************/
Data::Data(){
  voltage_11 = 11.1;
  voltage_22 = 22.2;
  relay_11 = 0;
  relay_22 = 0;
}

void Data::storeTime(unsigned long t){
  storedTime = t;
  storedMillis = millis();
}

unsigned long Data::retrieveTime(){
  unsigned long currentTime = storedTime + (millis() - storedMillis)/1000;
  return currentTime;
}

void Data::storeVoltage_11(float v){voltage_11 = v;}
void Data::storeVoltage_22(float v){voltage_22 = v;}
void Data::storeRelayStatus_11(bool r){relay_11 = r;}
void Data::storeRelayStatus_22(bool r){relay_22 = r;}

float Data::retrieveVoltage_11(){return voltage_11;}
float Data::retrieveVoltage_22(){return voltage_22;}
bool Data::retrieveRelay_11(){return relay_11;}
bool Data::retrieveRelay_22(){return relay_22;}
