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
Comm::Comm(bool actv, int fl, HardwareSerial *serPtr){
  active = actv;
  failLimit = fl;
  path = serPtr;
}

void Comm::sendJson(JsonDocument *doc){
  serializeJson(*doc, *path);
  path->println();    
  responseExpected = true;
}

void Comm::sendData(){
  if(responseExpected){
    failCount++;
    return;
  }
  StaticJsonDocument<256> doc;                      //NEED TO GET MORE ACCURATE ALLOCATION SIZE https://arduinojson.org/v6/assistant/

  doc["time"] = data.retrieveTime();
  doc["voltage"][0] = data.retrieveVoltage_11();
  doc["voltage"][1] = data.retrieveVoltage_22();
  doc["relay"][0] = data.retrieveRelay_11();
  doc["relay"][1] = data.retrieveRelay_22();
  if(usb.active)
    doc["comm status"][0] = usb.stat;
  if(wls.active)
    doc["comm status"][1] = wls.stat;

  Comm::sendJson(&doc);
}

void Comm::sendShutdown(char code[]){
  StaticJsonDocument<256> doc;                      //NEED TO GET MORE ACCURATE ALLOCATION SIZE https://arduinojson.org/v6/assistant/

  doc["time"] = data.retrieveTime();
  doc["SHUTDOWN"] = code;
  doc["voltage"][0] = data.retrieveVoltage_11();
  doc["voltage"][1] = data.retrieveVoltage_22();

  sendJson(&doc);
}

void Comm::requestTime(){
  StaticJsonDocument<256> doc;                      //NEED TO GET MORE ACCURATE ALLOCATION SIZE https://arduinojson.org/v6/assistant/
  doc["request"] = "time";
  sendJson(&doc);
}

void Comm::checkSerialBuffer(){
  if(path->available() < 1){
    if(responseExpected && failCount >= failLimit - 1){
      stat = false;
    }
    return;
  }
  parseSerial();
}

void Comm::parseSerial(){
  StaticJsonDocument<256> doc;                      //NEED TO GET MORE ACCURATE ALLOCATION SIZE https://arduinojson.org/v6/assistant/
  DeserializationError err = deserializeJson(doc, path->readString());
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
  
  if(responseExpected){
    if(doc["alive"]){
      responseExpected = false;
      stat = true;
      failCount = 0;
    }
  }

  if(doc["time"]){
    data.storeTime(doc["time"]);
  }
  // ADD COMMAND HANDLING HERE
}
