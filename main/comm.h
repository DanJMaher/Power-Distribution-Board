#ifndef COMM_H
#define COMM_H

#include "setup.h"
#include "data.h"
#include "supervision.h"

class Comm{
  public:
  // Serializes and transmits json doc to
  // specified Serial port
  static void sendJson(HardwareSerial *serPtr);

  // Checks buffer of specified serial port
  static void checkSerialBuffer(HardwareSerial *serPtr);

  private:
  // Checks for valid JSON data and parses that data
  static void parseSerial(HardwareSerial *serPtr);
};
#endif
