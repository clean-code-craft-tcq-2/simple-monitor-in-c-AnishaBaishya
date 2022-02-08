#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define ALL_OK		0
#define ALL_NOT_OK	1

char ParameterNames[3];
ParameterNames[]={"Temperature","SOC","Charge Rate"};

typedef enum {
  TempParameter,
  SOCParameter,
  ChargeRateParameter,
  NoOfParameter
} BatteryParameterList;

typedef struct {
  BatteryParameterList parameter;
  float minimumThreshold;
  float maximumThreshold;
  char parameterName[100];
} BatteryParameterInfo;

/* To keep track of Testcase number for ease of mapping testcase outcome to input parameters*/
int TestCaseCounter = 0;

void PopulateParameterInfo(){
  int counter;	
  int ParameterIndex = NoOfParameter;
  for (counter=0;counter<NoOfParameter;counter ++){ 
	  BatteryParameterInfo[ParameterIndex].parameter = BatteryParameterList[ParameterIndex];
	  BatteryParameterInfo[ParameterIndex].parameterName = ParameterNames[ParameterIndex];
}
	
void setRangeValues(char * ParameterName, float min, float max)
{
  int counter;	
  int ParameterIndex = NoOfParameter;
  for (counter=0;counter<NoOfParameter;counter ++){ 
	  if(BatteryParameterInfo[counter].parameterName == ParameterName)
	  ParameterIndex= BatteryParameterInfo[counter].parameter;
  BatteryParameterInfo[ParameterIndex].minimumThreshold= min;
  BatteryParameterInfo[ParameterIndex].maximumThreshold= max;
}

void printALLOk(char* BatteryParameter, int TestCaseCounter){
	printf("Testcase %d : Input %s within acceptable range\n",TestCaseCounter,BatteryParameter);
}

void printOnDisplay(float BatteryParameterValue, char* BatteryParameter,char* Condition, float ParameterThreshold,int TestCaseCounter) {
    printf("Testcase %d : Input %s of %f is %s than the threshold value of  %f\n",TestCaseCounter,BatteryParameter,BatteryParameterValue,Condition,ParameterThreshold);
}

bool isBatteryParameter_LessThanLowRange(float currentInput, BatteryParameterList BatteryParametersName) {
    bool MinThresholdCheck = 0;
    if(currentInput < BatteryParameterInfo[BatteryParametersName].minimumThreshold){
	    MinThresholdCheck=1;
	    printOnDisplay(currentInput,BatteryParameterInfo[BatteryParametersName].parameterName,"less",BatteryParameterInfo[BatteryParametersName].minimumThreshold,TestCaseCounter);
    return MinThresholdCheck;
}

bool isBatteryParameter_MoreThanHighRange(float currentInput, BatteryParameterList BatteryParametersName) {
    bool MaxThresholdCheck = 0;
    if(currentInput > BatteryParameterInfo[BatteryParametersName].maximumThreshold){
	    MaxThresholdCheck=1;
	    printOnDisplay(currentInput,BatteryParameterInfo[BatteryParametersName].parameterName,"more",BatteryParameterInfo[BatteryParametersName].maximumThreshold,TestCaseCounter);
    return MaxThresholdCheck;
}
}

bool isBatteryParametersWithinRange(BatteryParameterList BatteryParametersName,float currentInput){
	bool ParameterCheck;
	ParameterCheck= isBatteryParameter_LessThanLowRange(currentInput,BatteryParametersName);
	if(!ParameterCheck)
		ParameterCheck= isBatteryParameter_MoreThanHighRange(currentInput,BatteryParametersName);
	return ParameterCheck;		
}

int FetchParameterIndexFromName(char * ParameterName){
   int counter;	
   int ParameterIndex = NoOfParameter;
   for (counter=0;counter<NoOfParameter;counter ++){ 
	   if(BatteryParameterInfo[counter].parameterName == ParameterName)
		   ParameterIndex= BatteryParameterInfo[counter].parameter;
   return ParameterIndex;	
}
		
bool BatteryIsOk(float testData[]) {
   bool BatteryStatus = 0;
   int counter;	
   for (counter=0;counter<NoOfParameter;counter ++){
   BatteryStatus || = isBatteryParametersWithinRange(BatteryParameterInfo[counter].parameterName, testData[counter]);
   }
   return (BatteryStatus);  
}

void TestBatteryIsOk(bool expectedOutput,float testData[]){
   TestCaseCounter+=1;
   bool testBatteryStatus = BatteryIsOk(testData[]); 
   if(!testBatteryStatus)
	   printALLOk("parameters",TestCaseCounter);
   assert(testBatteryStatus==expectedOutput);
}

void TestBatteryParameterWithinRange(char BatteryParametersName, bool expectedOutput, float testParameter,){
   int ParameterIndex = FetchParameterIndexFromName(BatteryParametersName);
   bool testParameterStatus = isBatteryParametersWithinRange(ParameterIndex,testParameter);
   TestCaseCounter+=1;
   if(!testParameterStatus)
	printALLOk(BatteryParameterInfo[BatteryParametersName].parameterName,TestCaseCounter);
  }
   assert(testParameterStatus==expectedOutput);
}

int main() {
  float TestParameters[3];
	
  PopulateParameterInfo();
	  
  setRangeValues("Temperature",0.0,45.0);
  setRangeValues("SOC",20.0,80.0);
  setRangeValues("Charge Rate",0.0,0.8);
	
  TestParameters[]={25, 70, 0.7};
  TestBatteryIsOk(ALL_OK,TestParameters);
	
  TestParameters[]={50, 85, 0};
  TestBatteryIsOk(ALL_NOT_OK,TestParameters);
	
  setRangeValues("Temperature",10.0,30.0);
  TestBatteryParameterWithinRange("Temperature",ALL_NOT_OK,40.0);
	
  setRangeValues("SOC",10.0,70.0);
  TestBatteryParameterWithinRange("SOC",ALL_OK,40.0);
	
  setRangeValues("Charge Rate",0.0,0.6);
  TestBatteryParameterWithinRange("Charge Rate",ALL_NOT_OK,0.8);
	
  setRangeValues("Temperature",40.0,60.0);
  TestBatteryParameterWithinRange("Temperature",ALL_NOT_OK,30.0);
}
