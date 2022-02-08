#include <stdio.h>
#include <assert.h>

#define MINIMUM_TEMPERATURE    0.0
#define MAXIMUM_TEMPERATURE    45.0
#define MINIMUM_SOC    20.0
#define MAXIMUM_SOC    80.0
#define MINIMUM_CHARGERATE    0
#define MAXIMUM_CHARGERATE    0.8


int isBatteryParameter_OutOfRange(float currentInput, float minimumThreshold, float maximumThreshold) {
    return ((currentInput < minimumThreshold) || (currentInput > maximumThreshold));
}

int isTemperatureWithinRange(float currentTemperature) {
  int TemperatureStatus = isBatteryParameter_OutOfRange(currentTemperature, (float)MINIMUM_TEMPERATURE, (float)MAXIMUM_TEMPERATURE);
  if(TemperatureStatus)
    printf("Temperature out of range!\n");
  return TemperatureStatus;
}

int isSOCWithinRange(float currentSOC) {
  int SOCStatus = isBatteryParameter_OutOfRange(currentSOC, (float)MINIMUM_SOC, (float)MAXIMUM_SOC);
  if(SOCStatus)
      printf("State of Charge out of range!\n");
  return SOCStatus;
}

int isChargeRateWithinRange(float currentChargeRate) {
  int ChargeRateStatus = isBatteryParameter_OutOfRange(currentChargeRate, (float)MINIMUM_CHARGERATE, (float)MAXIMUM_CHARGERATE);
  if(ChargeRateStatus)
        printf("Charge Rate out of range!\n");
  return ChargeRateStatus;
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
