#ifndef COMM_H
#define COMM_H

#include "setup.h"
#include "supervision.h"

class Comm{
  public:
  static void createJson();
  static void sendJson(HardwareSerial *serPtr);
  static void checkSerialBuffer(HardwareSerial *serPtr);
  static void parseSerial(HardwareSerial *serPtr);
};
#endif
