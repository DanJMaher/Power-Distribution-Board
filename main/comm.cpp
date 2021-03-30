/**
 * comm.cpp
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

/******************************/
/**********HEADERS*************/
/******************************/
#include "comm.h"

/********************************/
/***********DEFINITIONS**********/
/********************************/
static void Comm::sendJson(HardwareSerial *serPtr, JsonDocument *doc){
  serializeJson(*doc, *serPtr);
  serPtr->println();    
  responseExpected = true;
}

static void Comm::sendData(HardwareSerial *serPtr){
  StaticJsonDocument<256> doc;                      //NEED TO GET MORE ACCURATE ALLOCATION SIZE https://arduinojson.org/v6/assistant/

  doc["time"] = data.retrieveTime();
  doc["voltage"][0] = data.retrieveVoltage_11();
  doc["voltage"][1] = data.retrieveVoltage_22();
  doc["relay"][0] = data.retrieveRelay_11();
  doc["relay"][1] = data.retrieveRelay_22();

  Comm::sendJson(serPtr, &doc);
}

static void Comm::sendShutdown(HardwareSerial *serPtr, char code[]){
  StaticJsonDocument<256> doc;                      //NEED TO GET MORE ACCURATE ALLOCATION SIZE https://arduinojson.org/v6/assistant/

  doc["time"] = data.retrieveTime();
  doc["SHUTDOWN"] = code;
  doc["voltage"][0] = data.retrieveVoltage_11();
  doc["voltage"][1] = data.retrieveVoltage_22();

  Comm::sendJson(serPtr, &doc);
}

static void Comm::requestTime(HardwareSerial *serPtr){
  StaticJsonDocument<256> doc;                      //NEED TO GET MORE ACCURATE ALLOCATION SIZE https://arduinojson.org/v6/assistant/
  doc["request"] = "time";
  Comm::sendJson(serPtr, &doc);
}

static void Comm::checkSerialBuffer(HardwareSerial *serPtr){
  if(serPtr->available() < 1)
    return;

  parseSerial(serPtr);
}

static void Comm::parseSerial(HardwareSerial *serPtr){
  StaticJsonDocument<256> doc;                      //NEED TO GET MORE ACCURATE ALLOCATION SIZE https://arduinojson.org/v6/assistant/
  DeserializationError err = deserializeJson(doc, serPtr->readString());
  if(err) {
    //ADD JSON FORMAT ERROR HANDLING HERE
    return;
  }

  if(doc["shutdown"] == "now"){
    char *shutdownCode = "remote";
    Supervision::powerDown(shutdownCode);
  }else if(doc["relay"]){
    digitalWrite(relayPin_11v, doc["relay"][0]);
    digitalWrite(relayPin_22v, doc["relay"][1]);
  }
  
  if(responseExpected)
    if(doc["alive"]){
      responseExpected = false;
      commFailCount = 0;
    }else if(commFailCount == commFailLimit){
      // COMM FAIL ACTION
    }else{
      commFailCount++;
  }

  if(doc["time"]){
    data.storeTime(doc["time"]);

  
  }
  // ADD COMMAND HANDLING HERE
}
