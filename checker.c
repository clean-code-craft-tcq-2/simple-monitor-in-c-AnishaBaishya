#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define ALL_OK		0
#define ALL_NOT_OK	1

/* To keep track of Testcase number for ease of mapping testcase outcome to input parameters*/
int TestCaseCounter = 0;

struct 
{
  float minimumThreshold;
  float maximumThreshold;
}Temperature;

struct 
{
  float minimumThreshold;
  float maximumThreshold;
}SOC;

struct 
{
  float minimumThreshold;
  float maximumThreshold;
}ChargeRate;

void setRangeforTemperature(float min, float max)
{
  Temperature.minimumThreshold= min;
  Temperature.maximumThreshold= max;
}

void setRangeforSOC(float min, float max)
{
  SOC.minimumThreshold= min;
  SOC.maximumThreshold= max;
}

void setRangeforChargeRate(float min, float max)
{
  ChargeRate.minimumThreshold= min;
  ChargeRate.maximumThreshold= max;
}

void printALLOk(char* BatteryParameter, int TestCaseCounter){
	printf("Testcase %d : Input %s within acceptable range\n",TestCaseCounter,BatteryParameter);
}

void printOnDisplay(float BatteryParameterValue, char* BatteryParameter,char* Condition, float ParameterThreshold,int TestCaseCounter) {
    printf("Testcase %d : Input %s of %f is %s than the threshold value of  %f\n",TestCaseCounter,BatteryParameter,BatteryParameterValue,Condition,ParameterThreshold);
}

bool isBatteryParameter_LessThanLowRange(float currentInput, float minimumThreshold) {
    return (currentInput < minimumThreshold);
}

bool isBatteryParameter_MoreThanHighRange(float currentInput, float maximumThreshold) {
    return (currentInput > maximumThreshold);
}

bool isTemperatureWithinRange(float currentTemperature) {
  bool TemperatureStatus = isBatteryParameter_LessThanLowRange(currentTemperature,Temperature.minimumThreshold);
  if(TemperatureStatus) 
  	printOnDisplay(currentTemperature,"Temperature","less",Temperature.minimumThreshold,TestCaseCounter);
  else{
  	TemperatureStatus = isBatteryParameter_MoreThanHighRange(currentTemperature,Temperature.maximumThreshold);
	if(TemperatureStatus)
  		printOnDisplay(currentTemperature,"Temperature","more",Temperature.maximumThreshold,TestCaseCounter);
      }
  return TemperatureStatus;
}

bool isSOCWithinRange(float currentSOC) {
  bool SOCStatus = isBatteryParameter_LessThanLowRange(currentSOC,SOC.minimumThreshold);
  if(SOCStatus)
  	printOnDisplay(currentSOC,"SOC","less",SOC.minimumThreshold,TestCaseCounter);
  else{
  	SOCStatus = isBatteryParameter_MoreThanHighRange(currentSOC,SOC.maximumThreshold);
	if(SOCStatus)
  		printOnDisplay(currentSOC,"SOC","more",SOC.maximumThreshold,TestCaseCounter);
	}
  return SOCStatus;
}

bool isChargeRateWithinRange(float currentChargeRate) {
  bool ChargeRateStatus = isBatteryParameter_LessThanLowRange(currentChargeRate,ChargeRate.minimumThreshold);
  if(ChargeRateStatus)
  	printOnDisplay(currentChargeRate,"ChargeRate","less",ChargeRate.minimumThreshold,TestCaseCounter);
  else{
  	ChargeRateStatus = isBatteryParameter_MoreThanHighRange(currentChargeRate,ChargeRate.maximumThreshold);
	if(ChargeRateStatus)
  		printOnDisplay(currentChargeRate,"ChargeRate","more",ChargeRate.maximumThreshold,TestCaseCounter);
	}
  return ChargeRateStatus;
}
bool BatteryIsOk(bool(*apBatteryChecks)(*apTestParameters)) {
   bool BatteryStatus = 0;
   int counter;	
   for (counter=0;counter<sizeof(apBatteryChecks);counter ++){
   BatteryStatus || = *apBatteryChecks[counter](apTestParameters[counter]);
   }
   return (BatteryStatus);  
}

void TestBatteryIsOk(bool expectedOutput,bool(*apBatteryChecks)(*apTestParameters)){
   TestCaseCounter+=1;
   bool testBatteryStatus = BatteryIsOk(apBatteryChecks(apTestParameters)); 
   if(!testBatteryStatus)
	   printALLOk("parameters",TestCaseCounter);
   assert(testBatteryStatus==expectedOutput);
}

void TestBatteryParameterWithinRange(char* BatteryParameter, bool expectedOutput, float testParameter,float minimumThreshold, float maximumThreshold){
   bool testParameterStatus = isBatteryParameter_LessThanLowRange(testParameter,minimumThreshold);
   TestCaseCounter+=1;
   if(testParameterStatus)
  	printOnDisplay(testParameter,BatteryParameter,"less",minimumThreshold,TestCaseCounter);
  else{
  	testParameterStatus = isBatteryParameter_MoreThanHighRange(testParameter,maximumThreshold);
	if(testParameterStatus) 
  		printOnDisplay(testParameter,BatteryParameter,"more",maximumThreshold,TestCaseCounter);
  	else
		printALLOk(BatteryParameter,TestCaseCounter);
  }
   assert(testParameterStatus==expectedOutput);
}

int main() {
  float TestParameters[3];
  bool (BatteryChecks[3])(float TestParameters[]);
	
  BatteryChecks[0] = isTemperatureWithinRange;
  BatteryChecks[1] = isSOCWithinRange;
  BatteryChecks[2] = isChargeRateWithinRange;
	
  setRangeforTemperature(0.0,45.0);
  setRangeforSOC(20.0,80.0);
  setRangeforChargeRate(0.0,0.8);
	
  TestParameters[3]={25, 70, 0.7};
  TestBatteryIsOk(ALL_OK,BatteryChecks(TestParameters));
	
  TestParameters[3]={50, 85, 0};
  TestBatteryIsOk(ALL_NOT_OK,&TestParameters[],&BatteryChecks);
	
  setRangeforTemperature(10.0,30.0);
  TestBatteryParameterWithinRange("Temperature",ALL_NOT_OK,40.0,Temperature.minimumThreshold, Temperature.maximumThreshold);
	
  setRangeforSOC(10.0,70.0);
  TestBatteryParameterWithinRange("SOC",ALL_OK,40.0,SOC.minimumThreshold, SOC.maximumThreshold);
	
  setRangeforChargeRate(0.0,0.6);
  TestBatteryParameterWithinRange("Charge Rate",ALL_NOT_OK,0.8,ChargeRate.minimumThreshold, ChargeRate.maximumThreshold);
	
  setRangeforTemperature(40.0,60.0);
  TestBatteryParameterWithinRange("Temperature",ALL_NOT_OK,30.0,Temperature.minimumThreshold, Temperature.maximumThreshold);
}
