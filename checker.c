#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define ALL_OK		0
#define ALL_NOT_OK	1

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

void printALLOk(char* BatteryParameter){
	printf("Input %s within acceptable range\n",BatteryParameter);
}

void printOnDisplay(float BatteryParameterValue, char* BatteryParameter,char* Condition, float ParameterThreshold) {
    printf("Input %s of %f is %s than the threshold value of  %f\n",BatteryParameter,BatteryParameterValue,Condition,ParameterThreshold);
}

bool isBatteryParameter_LessThanLowRange(float currentInput, float minimumThreshold) {
    return (currentInput < minimumThreshold);
}

bool isBatteryParameter_MoreThanHighRange(float currentInput, float maximumThreshold) {
    return (currentInput > maximumThreshold);
}

bool isTemperatureWithinRange(float currentTemperature) {
  bool TemperatureStatus = isBatteryParameter_LessThanLowRange(currentTemperature,Temperature.lowRange);
  if(TemperatureStatus) 
  	printOnDisplay(currentTemperature,"Temperature","less",Temperature.lowRange);
  else{
  	TemperatureStatus = isBatteryParameter_MoreThanHighRange(currentTemperature,Temperature.highRange);
	if(TemperatureStatus)
  		printOnDisplay(currentTemperature,"Temperature","more",Temperature.highRange);
      }
  return TemperatureStatus;
}

bool isSOCWithinRange(float currentSOC) {
  bool SOCStatus = isBatteryParameter_LessThanLowRange(currentSOC,SOC.lowRange);
  if(SOCStatus)
  	printOnDisplay(currentSOC,"SOC","less",SOC.lowRange);
  else{
  	SOCStatus = isBatteryParameter_MoreThanHighRange(currentSOC,SOC.highRange);
	if(SOCStatus)
  		printOnDisplay(currentSOC,"SOC","more",SOC.highRange);
	}
  return SOCStatus;
}

bool isChargeRateWithinRange(float currentChargeRate) {
  bool ChargeRateStatus = isBatteryParameter_LessThanLowRange(currentChargeRate,ChargeRate.lowRange);
  if(ChargeRateStatus)
  	printOnDisplay(currentChargeRate,"ChargeRate","less",ChargeRate.lowRange);
  else{
  	ChargeRateStatus = isBatteryParameter_MoreThanHighRange(currentChargeRate,ChargeRate.highRange);
	if(ChargeRateStatus)
  		printOnDisplay(currentChargeRate,"ChargeRate","more",ChargeRate.highRange);
	}
  return ChargeRateStatus;
}
bool BatteryIsOk(float currentTemperature, float currentSOC, float currentChargeRate) {
    return (isTemperatureWithinRange(currentTemperature) || isSOCWithinRange(currentSOC) || isChargeRateWithinRange(currentChargeRate));  
}

void TestBatteryIsOk(bool expectedOutput, float inputTemperature, float inputSOC, float inputChargeRate){															  
   bool testBatteryStatus = BatteryIsOk(inputTemperature, inputSOC, inputChargeRate);  
   if(!testBatteryStatus)
	   printALLOk("parameters");
   assert(testBatteryStatus==expectedOutput);
}

void TestBatteryParameterWithinRange(char* BatteryParameter, bool expectedOutput, float testParameter,float lowRange, float highRange){
   bool testParameterStatus = isBatteryParameter_LessThanLowRange(testParameter,lowRange);
   if(testParameterStatus)
  	printOnDisplay(testParameter,BatteryParameter,"less",lowRange);
  else{
  	testParameterStatus = isBatteryParameter_MoreThanHighRange(testParameter,highRange);
	if(testParameterStatus) 
  		printOnDisplay(testParameter,BatteryParameter,"more",highRange);
  	else
		printALLOk(BatteryParameter);
  }
   assert(testParameterStatus==expectedOutput);
}

int main() {
  setRangeforTemperature(0.0,45.0);
  setRangeforSOC(20.0,80.0);
  setRangeforChargeRate(0.0,0.8);
	
  TestBatteryIsOk(ALL_OK,25, 70, 0.7);
  TestBatteryIsOk(ALL_NOT_OK,50, 85, 0);
	
  setRangeforTemperature(10.0,30.0);
  TestBatteryParameterWithinRange("Temperature",ALL_NOT_OK,40.0,Temperature.lowRange, Temperature.highRange);
	
  setRangeforSOC(10.0,70.0);
  TestBatteryParameterWithinRange("SOC",ALL_OK,40.0,SOC.lowRange, SOC.highRange);
	
  setRangeforChargeRate(0.0,0.6);
  TestBatteryParameterWithinRange("Charge Rate",ALL_NOT_OK,0.8,ChargeRate.lowRange, ChargeRate.highRange);
}
