#include "data.h"

Data::Data(){
  voltage_11 = 11.1;
  voltage_22 = 22.2;
  relay_11 = 0;
  relay_22 = 0;
}
void Data::storeVoltage_11(float v){voltage_11 = v;}
void Data::storeVoltage_22(float v){voltage_22 = v;}
void Data::storeRelayStatus_11(bool r){relay_11 = r;}
void Data::storeRelayStatus_22(bool r){relay_22 = r;}

float Data::retrieveVoltage_11(){return voltage_11;}
float Data::retrieveVoltage_22(){return voltage_22;}
bool Data::retrieveRelay_11(){return relay_11;}
bool Data::retrieveRelay_22(){return relay_22;}
