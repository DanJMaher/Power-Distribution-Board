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
#include "supervision.h"

/********************************/
/**********DECLARATIONS**********/
/********************************/
class Comm{
  public:
  // Default Constructor
  Comm(bool actv, int fl, HardwareSerial *serPtr);
 
  // Converts data stored in the Data object to a json doc
  void sendData();

  // probably combine this one with sendData later.
  // Right now it's for special messages when a shutdown
  // is triggered.
  void sendShutdown(char code[]);

  // Creates a json document to request the time
  void requestTime();

  // Checks buffer of specified serial port
  void checkSerialBuffer();

  //private:
  // Serializes and transmits json doc to specified Serial port
  void sendJson(JsonDocument *doc);
  
  // Checks for valid JSON data and parses that data
  void parseSerial();

  // Bool to track if a message has been sent. but a response has not yet occurred.
  // Limit for system to track if comm fails are a problem.
  bool responseExpected = 0;
  int failCount = 0;
  int failLimit;
  HardwareSerial *path;
  bool active;
  bool stat = false;
  
};

#endif
