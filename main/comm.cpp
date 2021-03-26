#include "comm.h"

/** Records values to the json document
 */
static void Comm::createJson(){
  // Record current voltage
  transmitDoc["voltage"][0] = Supervision::readVoltage(monitorPin_11v, monitorCtrlPin_11v, maxVolts_11v);
  transmitDoc["voltage"][1] = Supervision::readVoltage(monitorPin_22v, monitorCtrlPin_22v, maxVolts_22v);

  // Record current relay status
  transmitDoc["relay"][0] = digitalRead(relayPin_11v);
  transmitDoc["relay"][1] = digitalRead(relayPin_22v);
}

/** Serializes and transmits json doc
 */
static void Comm::sendJson(HardwareSerial *serPtr){
    serializeJson(transmitDoc, *serPtr);
    serPtr->println();    
    responseExpected = true;
}

/** Checks the USB serial port for a command
 */
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
