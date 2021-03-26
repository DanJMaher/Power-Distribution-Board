#ifndef COMM_H
#define COMM_H

#include "setup.h"
#include "supervision.h"

class Comm{
  public:
  static void createJson();
  static void sendJson();
  static void checkSerialBuffer();
  static void parseSerial();
};
#endif
