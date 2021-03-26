#include "comm.h"

static void Comm::appendJson(String tag, int index, float value){
  transmitDoc[tag][index] = value;
}
static void Comm::appendJson(String tag, int index, bool value){
  transmitDoc[tag][index] = value;
}

static void Comm::sendJson(HardwareSerial *serPtr){
    serializeJson(transmitDoc, *serPtr);
    serPtr->println();    
    responseExpected = true;
}

static void Comm::checkSerialBuffer(HardwareSerial *serPtr){
  if(serPtr->available() < 1)
    return;

  parseSerial(serPtr);
}

static void Comm::parseSerial(HardwareSerial *serPtr){
  DeserializationError err = deserializeJson(receiveDoc, serPtr->readString());
  if(err) {
    //ADD ERROR HANDLING HERE
    return;
  }
  if(responseExpected)
    if(receiveDoc["alive"]){
      responseExpected = false;
      commFailCount = 0;
    }else if(commFailCount == commFailLimit){
      // COMM FAIL ACTION
    }else{
      commFailCount++;
    }
    
  // ADD COMMAND HANDLING HERE
}
