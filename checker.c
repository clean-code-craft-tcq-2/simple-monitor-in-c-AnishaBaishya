#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

struct 
{
  float lowRange;
  float highRange;
}Temperature;

struct 
{
  float lowRange;
  float highRange;
}SOC;

struct 
{
  float lowRange;
  float highRange;
}ChargeRate;

void setRangeforTemperature(float low, float high)
{
  Temperature.lowRange= low;
  Temperature.highRange= high;
}

void setRangeforSOC(float low, float high)
{
  SOC.lowRange= low;
  SOC.highRange= high;
}

void setRangeforChargeRate(float low, float high)
{
  ChargeRate.lowRange= low;
  ChargeRate.highRange= high;
}

void printOnDisplay(char *stringToBePrinted) {
    printf("%s \n", stringToBePrinted);
}

bool isBatteryParameter_OutOfRange(float currentInput, float minimumThreshold, float maximumThreshold) {
    return ((currentInput < minimumThreshold) || (currentInput > maximumThreshold));
}

bool isTemperatureWithinRange(float currentTemperature) {
  int TemperatureStatus = isBatteryParameter_OutOfRange(currentTemperature, Temperature.lowRange, Temperature.highRange);
  if(TemperatureStatus)
    printOnDisplay("Temperature out of range!");
  return TemperatureStatus;
}

bool isSOCWithinRange(float currentSOC) {
  int SOCStatus = isBatteryParameter_OutOfRange(currentSOC, SOC.lowRange, SOC.highRange);
  if(SOCStatus)
      printOnDisplay("State of Charge out of range!");
  return SOCStatus;
}

bool isChargeRateWithinRange(float currentChargeRate) {
  int ChargeRateStatus = isBatteryParameter_OutOfRange(currentChargeRate, ChargeRate.lowRange, ChargeRate.highRange);
  if(ChargeRateStatus)
        printOnDisplay("Charge Rate out of range!");
  return ChargeRateStatus;
}
bool BatteryIsOk(float currentTemperature, float currentSOC, float currentChargeRate) {
    return (isTemperatureWithinRange(currentTemperature) || isSOCWithinRange(currentSOC) || isChargeRateWithinRange(currentChargeRate));  
}

void TestBatteryIsOk(bool expectedOutput, float inputTemperature, float inputSOC, float inputChargeRate){															  
   bool testBatteryStatus = BatteryIsOk(inputTemperature, inputSOC, inputChargeRate);
   if(!testBatteryStatus)
	printOnDisplay("All Battery parameters ok!");   
   assert(testBatteryStatus==expectedOutput);
}

void TestBatteryParameterWithinRange(float testParameter,float lowRange, float highRange){
   bool testParameterStatus = isBatteryParameter_OutOfRange(testParameter,lowRange, highRange);
   if(testParameterStatus)
	printOnDisplay("Battery Parameter within range!");
   else
	printOnDisplay("Battery Parameter not within range!");
}

int main() {
  setRangeforTemperature(0.0,45.0);
  setRangeforSOC(20.0,80.0);
  setRangeforChargeRate(0.0,0.8);
	
  TestBatteryIsOk(0,25, 70, 0.7);
  TestBatteryIsOk(1,50, 85, 0);
	
  setRangeforTemperature(10.0,30.0);
  TestBatteryParameterWithinRange(40.0,Temperature.lowRange, Temperature.highRange);
	
  setRangeforSOC(10.0,70.0);
  TestBatteryParameterWithinRange(40.0,SOC.lowRange, SOC.highRange);
}
