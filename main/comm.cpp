#include "comm.h"

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
    //ADD ERROR HANDLING HERE
    return;
  }

  if(doc["time"]){
    data.storeTime(doc["time"]);
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
    
  // ADD COMMAND HANDLING HERE
}
