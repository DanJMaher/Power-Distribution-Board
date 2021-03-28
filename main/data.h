/**
 * data.h
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

#ifndef DATA_H
#define DATA_H

/******************************/
/**********HEADERS*************/
/******************************/
#include<Arduino.h>

/********************************/
/**********DECLARATIONS**********/
/********************************/
class Data{
  public:
  // Default constructor, doesn't do anything
  // important. Default values are overwritten
  // almost immediately
  Data();

  // Setter for the time retrieved from a host at boot
  void storeTime(unsigned long t);

  // Setters for the most recently read voltages
  // and relay statuses
  void storeVoltage_11(float v);
  void storeVoltage_22(float v);
  void storeRelayStatus_11(bool r);
  void storeRelayStatus_22(bool r);

  // Getter for the current system time
  unsigned long retrieveTime();

  // Getters for the most recently read voltages
  // and relay statuses
  float retrieveVoltage_11();
  float retrieveVoltage_22();
  bool retrieveRelay_11();
  bool retrieveRelay_22();

  private:
  // Storage for most recently read voltages and
  // relay statuses.
  float voltage_11;
  float voltage_22;
  bool relay_11;
  bool relay_22;

  // storedTime stores the time retrieved from a host
  // at boot. storedMillis is current system running time
  // when time was first retrieved. These values are used 
  // along with millis() to calculate the current time.
  unsigned long storedTime;
  unsigned long storedMillis;
};

#endif
