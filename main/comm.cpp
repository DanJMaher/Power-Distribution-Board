/**
   comm.cpp
 * *************************
   For Kent State's ATR Lab
   03/19/2021
   Dan Maher

   Code for operation of revision 1 of the custom
   power distribution board (PDB) PCB

 * *******************************************************
   The Comm (communication) class provides methods to
   generate JSON documents from stored data, send JSON
   documents via a serial stream, and monitor an
   incoming serial buffer.
 * *******************************************************
**/

/******************************/
/**********HEADERS*************/
/******************************/
#include "comm.h"

/********************************/
/***********DEFINITIONS**********/
/********************************/
Comm::Comm(bool actv, int fl, HardwareSerial *serPtr) {
  active = actv;
  failLimit = fl;
  path = serPtr;
}

void Comm::sendJson(JsonDocument *doc) {
  serializeJson(*doc, *path);
  path->println();
  responseExpected = true;
}

void Comm::sendData() {
  if (responseExpected) {
    failCount++;
    return;   // If uncommented, system does not try to send comm to a path that is currently
    // in the fail condition.
  }

  StaticJsonDocument<256> doc;                      //NEED TO GET MORE ACCURATE ALLOCATION SIZE https://arduinojson.org/v6/assistant/
  doc["msg"] = msgNo++;
  doc["time"] = data.retrieveTime();
  doc["voltage"][0] = data.getVoltage_11();
  doc["voltage"][1] = data.getVoltage_22();
  doc["relay"][0] = data.getRelay_11();
  doc["relay"][1] = data.getRelay_22();
  if (usb.active)
    doc["comm status"][0] = usb.stat;
  if (wls.active)
    doc["comm status"][1] = wls.stat;

  //doc["display status"] = Screen::displayStatus;

  Comm::sendJson(&doc);
}

void Comm::sendShutdown(char code[]) {
  StaticJsonDocument<256> doc;                      //NEED TO GET MORE ACCURATE ALLOCATION SIZE https://arduinojson.org/v6/assistant/
  doc["msg"] = msgNo++;
  doc["time"] = data.retrieveTime();
  doc["SHUTDOWN"] = code;
  doc["voltage"][0] = data.getVoltage_11();
  doc["voltage"][1] = data.getVoltage_22();
  doc["relay"][0] = data.getRelay_11();
  doc["relay"][1] = data.getRelay_22();

  sendJson(&doc);
}

void Comm::sendTestMessage() {
  StaticJsonDocument<256> doc;                      //NEED TO GET MORE ACCURATE ALLOCATION SIZE https://arduinojson.org/v6/assistant/
  doc["msg"] = msgNo++;
  doc["MESSAGE"] = "TEST";
  sendJson(&doc);
}

void Comm::requestTime() {
  StaticJsonDocument<256> doc;                      //NEED TO GET MORE ACCURATE ALLOCATION SIZE https://arduinojson.org/v6/assistant/
  doc["msg"] = msgNo++;
  doc["request"] = "time";
  sendJson(&doc);
}

void Comm::checkSerialBuffer() {
  if (path->available() < 1) {
    if (responseExpected && failCount >= failLimit - 1) {
      stat = false;
    }
    return;
  }
  parseSerial();
}

void Comm::parseSerial() {

  // Reads a character at a time from the serial buffer. Checks to see if it's { or } and then
  // adds it to a char array. After the '}' is received, it's deserialized into a JSON document
  // and parsed.
  char ch = path->read();

  if (commInProgress) {
    buf[bufIndex] = ch;
    if (ch == endChar) {
      commInProgress = false;
      bufIndex = 0;
    } else {
      bufIndex++;
      return;
    }
  } else {
    if (ch == startChar) {
      commInProgress = true;
      buf[bufIndex] = ch;
      bufIndex++;
      return;
    } else {
      return;
    }
  }

  StaticJsonDocument<256> doc;                      //NEED TO GET MORE ACCURATE ALLOCATION SIZE https://arduinojson.org/v6/assistant/
  DeserializationError err = deserializeJson(doc, buf);
  if (err) {
    //Serial.println("ERROR");
    return;
  }

  if (doc["shutdown"] == "now") {
    char *shutdownCode = "remote";
    Supervision::powerDown(shutdownCode);
  } else if (doc["relay"]) {
    Supervision::setRelay(relayPin_11v, doc["relay"][0]);
    Supervision::setRelay(relayPin_22v, doc["relay"][1]);
  }

  if (doc["alive"]) {
    responseExpected = false;
    stat = true;
    failCount = 0;
  }

  if (doc["time"]) {
    data.storeTime(doc["time"]);
  }
}
