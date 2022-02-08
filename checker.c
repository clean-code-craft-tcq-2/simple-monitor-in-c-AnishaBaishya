#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define MINIMUM_TEMPERATURE    0.0
#define MAXIMUM_TEMPERATURE    45.0
#define MINIMUM_SOC    20.0
#define MAXIMUM_SOC    80.0
#define MINIMUM_CHARGERATE    0
#define MAXIMUM_CHARGERATE    0.8

void printOnDisplay(char *stringToBePrinted) {
    printf("%s \n", stringToBePrinted);
}

bool isBatteryParameter_OutOfRange(float currentInput, float minimumThreshold, float maximumThreshold) {
    return ((currentInput < minimumThreshold) || (currentInput > maximumThreshold));
}

bool isTemperatureWithinRange(float currentTemperature) {
  int TemperatureStatus = isBatteryParameter_OutOfRange(currentTemperature, (float)MINIMUM_TEMPERATURE, (float)MAXIMUM_TEMPERATURE);
  if(TemperatureStatus)
    printOnDisplay("Temperature out of range!");
  return TemperatureStatus;
}

bool isSOCWithinRange(float currentSOC) {
  int SOCStatus = isBatteryParameter_OutOfRange(currentSOC, (float)MINIMUM_SOC, (float)MAXIMUM_SOC);
  if(SOCStatus)
      printOnDisplay("State of Charge out of range!");
  return SOCStatus;
}

bool isChargeRateWithinRange(float currentChargeRate) {
  int ChargeRateStatus = isBatteryParameter_OutOfRange(currentChargeRate, (float)MINIMUM_CHARGERATE, (float)MAXIMUM_CHARGERATE);
  if(ChargeRateStatus)
        printOnDisplay("Charge Rate out of range!");
  return ChargeRateStatus;
}
bool BatteryIsOk(float currentTemperature, float currentSOC, float currentChargeRate) {
    return (isTemperatureWithinRange(currentTemperature) || isSOCWithinRange(currentSOC) || isChargeRateWithinRange(currentChargeRate));  
}

void TestBatteryIsOk(bool expectedOutput, float inputTemperature, float inputSOC, float inputChargeRate){
   bool testBatteryStatus = BatteryIsOk(inputTemperature, inputSOC, inputChargeRate);
   assert(testBatteryStatus==expectedOutput);
}

int main() {
  TestBatteryIsOk(0,25, 70, 0.7);
  TestBatteryIsOk(1,50, 85, 0);
}
