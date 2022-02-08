#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define ALL_OK		0
#define ALL_NOT_OK	1

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

BatteryParameterInfo parameterInfo [NoOfParameter] ;

void PopulateParameterInfo(){
  	parameterInfo[0].parameter = TempParameter;
	strcpy(parameterInfo[0].parameterName,"Temperature");
	parameterInfo[1].parameter = SOCParameter;
	strcpy(parameterInfo[1].parameterName, "SOC");
	parameterInfo[2].parameter = ChargeRateParameter;
	strcpy(parameterInfo[2].parameterName, "Charge Rate");  
}
	
void setRangeValues(char * ParameterName, float min, float max)
{
  int counter;	
  int ParameterIndex = NoOfParameter;
  for (counter=0;counter<NoOfParameter;counter ++){ 
	  if(parameterInfo[counter].parameterName == ParameterName){
	  	ParameterIndex= parameterInfo[counter].parameter;
  		parameterInfo[counter].minimumThreshold= min;
  		parameterInfo[counter].maximumThreshold= max;
	  }
	  }
}

void printALLOk(char* BatteryParameter, int TestCaseCounter){
	printf("Testcase %d : Input %s within acceptable range\n",TestCaseCounter,BatteryParameter);
}

void printOnDisplay(float BatteryParameterValue, char* BatteryParameter,char* Condition, float ParameterThreshold,int TestCaseCounter) {
    printf("Testcase %d : Input %s of %f is %s than the threshold value of  %f\n",TestCaseCounter,BatteryParameter,BatteryParameterValue,Condition,ParameterThreshold);
}

bool isBatteryParameter_LessThanLowRange(float currentInput, BatteryParameterList BatteryParametersName) {
    bool MinThresholdCheck = 0;
    if(currentInput < parameterInfo[BatteryParametersName].minimumThreshold){
	    MinThresholdCheck=1;
	    printOnDisplay(currentInput,parameterInfo[BatteryParametersName].parameterName,"less",parameterInfo[BatteryParametersName].minimumThreshold,TestCaseCounter);
    }
	printf("MinThresholdCheck %d\n",MinThresholdCheck);
    return MinThresholdCheck;
}

bool isBatteryParameter_MoreThanHighRange(float currentInput, BatteryParameterList BatteryParametersName) {
    bool MaxThresholdCheck = 0;
    if(currentInput > parameterInfo[BatteryParametersName].maximumThreshold){
	    MaxThresholdCheck=1;
	    printOnDisplay(currentInput,parameterInfo[BatteryParametersName].parameterName,"more",parameterInfo[BatteryParametersName].maximumThreshold,TestCaseCounter);
    }
	printf("MaxThresholdCheck %d\n",MaxThresholdCheck);
    return MaxThresholdCheck;
}

bool isBatteryParametersWithinRange(BatteryParameterList BatteryParametersName,float currentInput){
	bool ParameterCheck;
	ParameterCheck= isBatteryParameter_LessThanLowRange(currentInput,BatteryParametersName);
	printf("ParameterCheckLow %d\n",ParameterCheck);
	if(!ParameterCheck)
		ParameterCheck= isBatteryParameter_MoreThanHighRange(currentInput,BatteryParametersName);
		printf("ParameterCheckHigh %d\n",ParameterCheck);
	return ParameterCheck;		
}

int FetchParameterIndexFromName(char ParameterName){
   int counter;	
   int ParameterIndex = NoOfParameter;
   for (counter=0;counter<NoOfParameter;counter ++){ 
	   if(parameterInfo[counter].parameterName == ParameterName)
		   ParameterIndex= parameterInfo[counter].parameter;
   }
   return ParameterIndex;	
}
		
bool BatteryIsOk(float testData[]) {
   bool BatteryStatus = 0;
   int counter;	
   for (counter=0;counter<NoOfParameter;counter ++){
   BatteryStatus|=isBatteryParametersWithinRange(parameterInfo[counter].parameter, testData[counter]);
	   printf("BatteryStatus %d\n",BatteryStatus);
   }
   return (BatteryStatus);  
}

void TestBatteryIsOk(bool expectedOutput,float testData[]){
   TestCaseCounter+=1;
   bool testBatteryStatus = BatteryIsOk(testData); 
	printf("testBatteryStatus %d\n",testBatteryStatus);
   if(!testBatteryStatus)
	   printALLOk("parameters",TestCaseCounter);
//    assert(testBatteryStatus==expectedOutput);
}

void TestBatteryParameterWithinRange(char BatteryParametersName, bool expectedOutput, float testParameter){
   int ParameterIndex = FetchParameterIndexFromName(BatteryParametersName);
   bool testParameterStatus = isBatteryParametersWithinRange(ParameterIndex,testParameter);
   TestCaseCounter+=1;
   if(!testParameterStatus)
	printALLOk(parameterInfo[BatteryParametersName].parameterName,TestCaseCounter);
//    assert(testParameterStatus==expectedOutput);
}

int main() {
  PopulateParameterInfo();
	  
  setRangeValues("Temperature",0.0,45.0);
  setRangeValues("SOC",20.0,80.0);
  setRangeValues("Charge Rate",0.0,0.8);
	
  printf("Temp %f %f %s\n",parameterInfo[0].minimumThreshold,parameterInfo[0].maximumThreshold,parameterInfo[0].parameterName);
	printf("SOC %f %f %s\n",parameterInfo[1].minimumThreshold,parameterInfo[1].maximumThreshold,parameterInfo[1].parameterName);
	printf("Charge %f %f %s\n",parameterInfo[2].minimumThreshold,parameterInfo[2].maximumThreshold,parameterInfo[2].parameterName);
	
  float TestParameters1[3]={25, 70, 0.7};
  printf("hello %f",TestParameters1);
  TestBatteryIsOk(ALL_OK,TestParameters1);
	
//   float TestParameters2[]={50, 85, 0};
//   TestBatteryIsOk(ALL_NOT_OK,TestParameters2);
	
  setRangeValues("Temperature",10.0,30.0);
  TestBatteryParameterWithinRange("Temperature",ALL_NOT_OK,40.0);
	
  setRangeValues("SOC",10.0,70.0);
  TestBatteryParameterWithinRange("SOC",ALL_OK,40.0);
	
  setRangeValues("Charge Rate",0.0,0.6);
  TestBatteryParameterWithinRange("Charge Rate",ALL_NOT_OK,0.8);
	
  setRangeValues("Temperature",40.0,60.0);
  TestBatteryParameterWithinRange("Temperature",ALL_NOT_OK,30.0);
}
