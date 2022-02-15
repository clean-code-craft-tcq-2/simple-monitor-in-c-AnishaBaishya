#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define ALL_OK				0
#define ALL_NOT_OK			1

#define EARLY_WARNING_SUPPORTED		1
#define EARLY_WARNING_NOTSUPPORTED	0

#define WARNINGTOLERANCE 		5

typedef enum {
  TempParameter,
  SOCParameter,
  ChargeRateParameter,
  NoOfParameter
} EV_BatteryParameterTypesForBMS;

typedef struct {
  EV_BatteryParameterTypesForBMS parameter;
  float minimumThreshold;
  float maximumThreshold;
  bool WarningSupported;
  char parameterName[100];
} BatteryParameterInfo;

typedef struct {
	float LowValueOfWarningTolerance;
	float HighValueOfWarningTolerance;
}BatteryParameterToleranceValues;

int StringsAreEqual(char * String1,char * String2){
	return (!(strcmp(String1,String2)));
}
/* To keep track of Testcase number for ease of mapping testcase outcome to input parameters*/
int TestCaseCounter = 0;

BatteryParameterInfo parameterInfo [NoOfParameter] ;

void PopulateParameterInfo(){
  	parameterInfo[0].parameter = TempParameter;
	strcpy(parameterInfo[0].parameterName,"Temperature");
	parameterInfo[0].WarningSupported = EARLY_WARNING_SUPPORTED;
	parameterInfo[1].parameter = SOCParameter;
	strcpy(parameterInfo[1].parameterName, "SOC");
	parameterInfo[1].WarningSupported = EARLY_WARNING_SUPPORTED;
	parameterInfo[2].parameter = ChargeRateParameter;
	strcpy(parameterInfo[2].parameterName, "Charge Rate"); 
	parameterInfo[2].WarningSupported = EARLY_WARNING_SUPPORTED;
}
	
void setRangeValues(char* ParameterName, float min, float max)
{
  int counter;	
  for (counter=0;counter<NoOfParameter;counter ++){ 
	  if(StringsAreEqual(parameterInfo[counter].parameterName,ParameterName)){
  		parameterInfo[counter].minimumThreshold= min;
  		parameterInfo[counter].maximumThreshold= max;
	  }
	  }
}

void setToleranceLimitValues(EV_BatteryParameterTypesForBMS BatteryParametersName, BatteryParameterToleranceValues* currentParamaterToleranceValues){
	float ToleranceValueForBatteryParameter = ((WARNINGTOLERANCE/100)*(parameterInfo[BatteryParametersName].maximumThreshold));
	currentParamaterToleranceValues->LowValueOfWarningTolerance = parameterInfo[BatteryParametersName].minimumThreshold + ToleranceValueForBatteryParameter;
	currentParamaterToleranceValues->HighValueOfWarningTolerance = parameterInfo[BatteryParametersName].maximumThreshold - ToleranceValueForBatteryParameter;
}

void printALLOk(char* BatteryParameter, int TestCaseCounter){
	printf("Testcase %d : Input %s within acceptable normal range\n",TestCaseCounter,BatteryParameter);
}

void printOnDisplayLimitBreached(char* BatteryParameter,char* Condition,int TestCaseCounter) {
    printf("Testcase %d : CAUTION ! Input %s is %s than the threshold value. Take appropriate action\n",TestCaseCounter,BatteryParameter,Condition);
}

void printOnDisplayToleranceLimitApproached(char* BatteryParameter,char* Condition,int TestCaseCounter) {
    printf("Testcase %d : WARNING ! Input %s is approaching the %s threshold value. Take appropriate action\n",TestCaseCounter,BatteryParameter,Condition);
}

bool isBatteryParameter_LessThanLowRange(float currentInput, EV_BatteryParameterTypesForBMS BatteryParametersName) {
    bool MinThresholdCheck = 0;
    if(currentInput < parameterInfo[BatteryParametersName].minimumThreshold){
	    MinThresholdCheck=1;
	    printOnDisplayLimitBreached(parameterInfo[BatteryParametersName].parameterName,"less",TestCaseCounter);
    }
    return MinThresholdCheck;
}

bool isBatteryParameter_MoreThanHighRange(float currentInput, EV_BatteryParameterTypesForBMS BatteryParametersName) {
    bool MaxThresholdCheck = 0;
    if(currentInput > parameterInfo[BatteryParametersName].maximumThreshold){
	    MaxThresholdCheck=1;
	    printOnDisplayLimitBreached(parameterInfo[BatteryParametersName].parameterName,"more",TestCaseCounter);
    }
    return MaxThresholdCheck;
}

bool isBatteryParameter_InLowerToleranceLimitRange(float currentInput, EV_BatteryParameterTypesForBMS BatteryParametersName, float lowerToleranceLimit){
	bool MinToleranceLimitCheck = 0;
	if(currentInput<=lowerToleranceLimit){
		MinToleranceLimitCheck=1;
		printOnDisplayToleranceLimitApproached(parameterInfo[BatteryParametersName].parameterName,"lower",TestCaseCounter);
	}
	return MinToleranceLimitCheck;
}

bool isBatteryParameter_InHigherToleranceLimitRange(float currentInput, EV_BatteryParameterTypesForBMS BatteryParametersName, float higherToleranceLimit){
	bool MaxToleranceLimitCheck = 0;
	if(currentInput>=higherToleranceLimit){
		MaxToleranceLimitCheck=1;
		printOnDisplayToleranceLimitApproached(parameterInfo[BatteryParametersName].parameterName,"higher",TestCaseCounter);
	}
	return MaxToleranceLimitCheck;
}

bool isBatteryParametersWithinToleranceLimit(EV_BatteryParameterTypesForBMS BatteryParametersName,float currentInput){
	bool ParameterCheck;
	BatteryParameterToleranceValues currentParamaterToleranceValues;
	setToleranceLimitValues(BatteryParametersName,&currentParamaterToleranceValues);
	ParameterCheck= isBatteryParameter_InLowerToleranceLimitRange(currentInput,BatteryParametersName,currentParamaterToleranceValues.LowValueOfWarningTolerance);
	if(!ParameterCheck)
		ParameterCheck= isBatteryParameter_InHigherToleranceLimitRange(currentInput,BatteryParametersName,currentParamaterToleranceValues.HighValueOfWarningTolerance);
	return ParameterCheck;
}

bool isBatteryParametersWithinNormalRange(EV_BatteryParameterTypesForBMS BatteryParametersName,float currentInput){
	bool ParameterCheck;
	ParameterCheck= isBatteryParameter_LessThanLowRange(currentInput,BatteryParametersName);
	if(!ParameterCheck)
		ParameterCheck= isBatteryParameter_MoreThanHighRange(currentInput,BatteryParametersName);
	return ParameterCheck;		
}

int FetchParameterIndexFromName(char* ParameterName){
   int counter;	
   int ParameterIndex = NoOfParameter;
   for (counter=0;counter<NoOfParameter;counter ++){ 
	   if(StringsAreEqual(parameterInfo[counter].parameterName,ParameterName)){
		   ParameterIndex= parameterInfo[counter].parameter;
	   }
   }
   return ParameterIndex;	
}
		
void BatteryIsOk(float testData[], bool* BatteryErrorStatus, bool* BatteryWarningStatus ) {
   int counter;	
   for (counter=0;counter<NoOfParameter;counter ++){
	   *BatteryErrorStatus|=isBatteryParametersWithinNormalRange(parameterInfo[counter].parameter, testData[counter]);
	   if(parameterInfo[counter].WarningSupported)
		*BatteryWarningStatus|=isBatteryParametersWithinToleranceLimit(parameterInfo[counter].parameter, testData[counter]); 
   } 
}

void TestBatteryIsOk(bool expectedOutput,float testData[]){
   TestCaseCounter+=1;
   bool testBatteryStatus;
   bool BatteryErrorStatus = 0;
   bool BatteryWarningStatus = 0;
   BatteryIsOk(testData,&BatteryErrorStatus,&BatteryWarningStatus); 
   if(BatteryErrorStatus == 1 || BatteryWarningStatus ==1)
	   testBatteryStatus = ALL_NOT_OK;
   else{
	   testBatteryStatus = ALL_OK;
	   printALLOk("parameters",TestCaseCounter);
   }
   assert(testBatteryStatus==expectedOutput);
}

void TestBatteryParameterWithinRange(char* BatteryParametersName, bool expectedOutput, float testParameter){
   int ParameterIndex = FetchParameterIndexFromName(BatteryParametersName);
   TestCaseCounter+=1;
   bool testParameterStatus = isBatteryParametersWithinNormalRange(ParameterIndex,testParameter);
   if(!testParameterStatus)
	printALLOk(BatteryParametersName,TestCaseCounter);
   assert(testParameterStatus==expectedOutput);
}

int main() {
  PopulateParameterInfo();
	  
  setRangeValues("Temperature",0.0,45.0);
  setRangeValues("SOC",20.0,80.0);
  setRangeValues("Charge Rate",0.0,0.8);
	
// //   Testcase 1
//   float TestParameters1[3]={25.0, 70.0, 0.7};
//   TestBatteryIsOk(ALL_OK,TestParameters1);
	
// //   Testcase 2
//   float TestParameters2[3]={50.0, 85.0, 0};
//   TestBatteryIsOk(ALL_NOT_OK,TestParameters2);
	
// //   Testcase 3
//   float TestParameters3[3]={40, 10, 0.9};
//   TestBatteryIsOk(ALL_NOT_OK,TestParameters3);
	
//   Testcase 4
  float TestParameters4[3]={43.0, 21.0, 0.4};
  TestBatteryIsOk(ALL_NOT_OK,TestParameters4);

// //   Testcase 4
//   setRangeValues("Temperature",10.0,30.0);
//   TestBatteryParameterWithinRange("Temperature",ALL_NOT_OK,40.0);
	
// //   Testcase 5
//   setRangeValues("SOC",10.0,70.0);
//   TestBatteryParameterWithinRange("SOC",ALL_OK,40.0);
	
// //   Testcase 6
//   setRangeValues("Charge Rate",0.0,0.6);
//   TestBatteryParameterWithinRange("Charge Rate",ALL_NOT_OK,0.8);
	
// //   Testcase 7
//   setRangeValues("Temperature",40.0,60.0);
//   TestBatteryParameterWithinRange("Temperature",ALL_NOT_OK,30.0);
}
