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
#include "screen.h"

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

  // Triggered by user menu to send a communication test
  void sendTestMessage();

  // Creates a json document to request the time
  void requestTime();

  // Checks buffer of specified serial port
  void checkSerialBuffer();

  // Serializes and transmits json doc to specified Serial port
  void sendJson(JsonDocument *doc);
  
  // Checks for valid JSON data and parses that data
  void parseSerial();

  // Bool to track if a message has been sent. but a response has not yet occurred.
  bool responseExpected = 0;
  // Tracks how many comm fails have occured on this path
  int failCount = 0;
  // Limit for system to track if comm fails are a problem.
  int failLimit;
  // Tracks whether the path is currently in a fail state (failLimit has been reached)
  bool stat = false;
  // Serial path that the particular object relys on
  HardwareSerial *path;
  // True if the serial path is currently being used. Stored in EEPROM
  bool active;
  // Char array that holds the active incoming Serial buffer
  char buf[256];                                                                                // NEED TO CALCULATE MORE ACCURATE SIZE
  // Holds the current char array index of the active incoming Serial buffer
  int bufIndex = 0;
  // True if there is currently a message in progress. This is known because the '{' char
  // has been received
  bool commInProgress = false;
  const char startChar = '{';
  const char endChar = '}';
  // Message number for host tracking
  int msgNo = 0;
};

#endif
