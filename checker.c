#include <stdio.h>
#include <assert.h>

#define MINIMUM_TEMPERATURE    0.0
#define MAXIMUM_TEMPERATURE    45.0
#define MINIMUM_SOC    20.0
#define MAXIMUM_SOC    80.0
#define MAXIMUM_CHARGERATE    0.8

int isTemperatureWithinRange(float currentTemperature) {
  if(currentTemperature < MINIMUM_TEMPERATURE || currentTemperature > MAXIMUM_TEMPERATURE) {
    printf("Temperature out of range!\n");
    return 0;
  }
  else
    return 1
}

int isSOCWithinRange(float currentSOC) {
    if(currentSOC < MINIMUM_SOC || currentSOC > MAXIMUM_SOC) {
      printf("State of Charge out of range!\n");
      return 0;
    }
    else
      return 1;
}

bool isChargeRateWithinRange(float currentChargeRate) {
    if(currentChargeRate > MAXIMUM_CHARGERATE) {
      printf("Charge Rate out of range!\n");
      return 0;
    }
    else
      return 1;
}
int BatteryIsOk(float currentTemperature, float currentSOC, float currentChargeRate) {
    return (isTemperatureWithinRange(currentTemperature) & isSOCWithinRange(currentSOC) & isChargeRateWithinRange(currentChargeRate));  
}

int TestBatteryIsOk(float inputTemperature, float inputSOC, float inputChargeRate){
  return BatteryIsOk(inputTemperature, inputSOC, inputChargeRate);
}

int main() {
  assert(TestBatteryIsOk(25, 70, 0.7));
  assert(!TestBatteryIsOk(50, 85, 0));
}
