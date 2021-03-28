/**
 * comm.h
 * *************************
 * For Kent State's ATR Lab  
 * 03/19/2021
 * Dan Maher
 * 
 * Code for operation of revision 1 of the custom 
 * power distribution board (PDB) PCB 
 * 
 * *******************************************************
 * The Comm (communication) class provides methods to 
 * generate JSON documents from stored data, send JSON
 * documents via a serial stream, and monitor an 
 * incoming serial buffer.
 * *******************************************************
**/

#ifndef COMM_H
#define COMM_H

/******************************/
/**********HEADERS*************/
/******************************/
#include "setup.h"
#include "data.h"
#include "supervision.h"

/********************************/
/**********DECLARATIONS**********/
/********************************/
class Comm{
  public:
  // Converts data stored in the Data object to a json doc
  static void sendData(HardwareSerial *serPtr);

  // Creates a json document to request the time
  static void requestTime(HardwareSerial *serPtr);

  // Checks buffer of specified serial port
  static void checkSerialBuffer(HardwareSerial *serPtr);

  private:
  // Serializes and transmits json doc to specified Serial port
  static void sendJson(HardwareSerial *serPtr, JsonDocument *doc);
  
  // Checks for valid JSON data and parses that data
  static void parseSerial(HardwareSerial *serPtr);
};

#endif
