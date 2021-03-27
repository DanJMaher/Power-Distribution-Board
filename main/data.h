#ifndef DATA_H
#define DATA_H

#include<Arduino.h>

class Data{
  public:
  Data();

  void storeTime(unsigned long t);
  
  void storeVoltage_11(float v);
  void storeVoltage_22(float v);
  void storeRelayStatus_11(bool r);
  void storeRelayStatus_22(bool r);

  unsigned long retrieveTime();
  
  float retrieveVoltage_11();
  float retrieveVoltage_22();
  bool retrieveRelay_11();
  bool retrieveRelay_22();

  private:
  float voltage_11;
  float voltage_22;
  bool relay_11;
  bool relay_22;

  unsigned long storedTime;
  unsigned long storedMillis;
};

#endif
