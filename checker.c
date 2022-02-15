#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
  ENGLISH,
  GERMAN
} SupportedLanguages;

#define DISPLAYMESSAGE_LANGUAGE		ENGLISH
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

typedef struct{
	float Temperature,
	char TempUnit,
	float SOC,
	float ChargeRate
}Struct_BatteryTestData;

int StringsAreEqual(char * String1,char * String2){
	return (!(strcmp(String1,String2)));
}
/* To keep track of Testcase number for ease of mapping testcase outcome to input parameters*/
int TestCaseCounter = 0;

BatteryParameterInfo parameterInfo [NoOfParameter] ;

typedef struct {
char messageAllOk[50];
char messageLimitBreached[3][50];
char messageToleranceLimitApproached[3][50];
}DifferentDisplayMessage;

typedef struct{
char ParameterName[NoOfParameter][20];
}BatteryParameterNameList;

BatteryParameterNameList BatteryParameterName;
DifferentDisplayMessage DisplayMessageBasedOnLanguageChosen;
void DisplayMessageBasedOnSupportedLanguage(){
	if(DISPLAYMESSAGE_LANGUAGE == ENGLISH)	
	{
		strcpy(BatteryParameterName.ParameterName[0] ,"Temperature");
		strcpy(BatteryParameterName.ParameterName[1] ,"State of Charge");
		strcpy(BatteryParameterName.ParameterName[2] ,"Charge Rate");
		strcpy(DisplayMessageBasedOnLanguageChosen.messageAllOk ,"Test parameter(s) within acceptable normal range");
		strcpy(DisplayMessageBasedOnLanguageChosen.messageLimitBreached[0],"CAUTION ! ");
		strcpy(DisplayMessageBasedOnLanguageChosen.messageLimitBreached[1], " is less than threshold value.");
		strcpy(DisplayMessageBasedOnLanguageChosen.messageLimitBreached[2]," is more than threshold value.");
		strcpy(DisplayMessageBasedOnLanguageChosen.messageToleranceLimitApproached[0], "WARNING ! ");
		strcpy(DisplayMessageBasedOnLanguageChosen.messageToleranceLimitApproached[1]," is approaching the lower threshold value.");
		strcpy(DisplayMessageBasedOnLanguageChosen.messageToleranceLimitApproached[2], " is approaching the higher threshold value.");
	}
	if(DISPLAYMESSAGE_LANGUAGE == GERMAN)	
	{
		strcpy(BatteryParameterName.ParameterName[0] ,"Temperatur");
		strcpy(BatteryParameterName.ParameterName[1] ,"Ladezustand");
		strcpy(BatteryParameterName.ParameterName[2] ,"Ladestrom");
		strcpy(DisplayMessageBasedOnLanguageChosen.messageAllOk ,"Testparameter innerhalb eines akzeptablen Normalbereichs");
		strcpy(DisplayMessageBasedOnLanguageChosen.messageLimitBreached[0],"VORSICHT ! ");
		strcpy(DisplayMessageBasedOnLanguageChosen.messageLimitBreached[1], " kleiner als der Schwellenwert ist.");
		strcpy(DisplayMessageBasedOnLanguageChosen.messageLimitBreached[2]," über dem Schwellenwert liegt.");
		strcpy(DisplayMessageBasedOnLanguageChosen.messageToleranceLimitApproached[0], "WARNUNG ! ");
		strcpy(DisplayMessageBasedOnLanguageChosen.messageToleranceLimitApproached[1]," nähert sich dem unteren Schwellenwert.");
		strcpy(DisplayMessageBasedOnLanguageChosen.messageToleranceLimitApproached[2], " nähert sich dem höheren Schwellenwert.");
	}
}

void PopulateParameterInfo(){
  	parameterInfo[0].parameter = TempParameter;
	strcpy(parameterInfo[0].parameterName,BatteryParameterName.ParameterName[0]);
	parameterInfo[0].WarningSupported = EARLY_WARNING_SUPPORTED;
	parameterInfo[1].parameter = SOCParameter;
	strcpy(parameterInfo[1].parameterName,BatteryParameterName.ParameterName[1]);
	parameterInfo[1].WarningSupported = EARLY_WARNING_SUPPORTED;
	parameterInfo[2].parameter = ChargeRateParameter;
	strcpy(parameterInfo[2].parameterName,BatteryParameterName.ParameterName[2]); 
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
	float ToleranceValueForBatteryParameter = ((((float)WARNINGTOLERANCE)/100)*(parameterInfo[BatteryParametersName].maximumThreshold));
	currentParamaterToleranceValues->LowValueOfWarningTolerance = parameterInfo[BatteryParametersName].minimumThreshold + ToleranceValueForBatteryParameter;
	currentParamaterToleranceValues->HighValueOfWarningTolerance = parameterInfo[BatteryParametersName].maximumThreshold - ToleranceValueForBatteryParameter;
}

void printALLOk(int TestCaseCounter){
	printf("%d : %s\n",TestCaseCounter,DisplayMessageBasedOnLanguageChosen.messageAllOk);
}

void printOnDisplayLimitBreached(char* BatteryParameter,char* MessageBody,int TestCaseCounter) {
    printf("%d : %s%s%s\n",TestCaseCounter,DisplayMessageBasedOnLanguageChosen.messageLimitBreached[0],BatteryParameter,MessageBody);
}

void printOnDisplayToleranceLimitApproached(char* BatteryParameter,char* MessageBody,int TestCaseCounter) {
    printf("%d : %s%s%s\n",TestCaseCounter,DisplayMessageBasedOnLanguageChosen.messageToleranceLimitApproached[0],BatteryParameter,MessageBody);
}

bool isBatteryParameter_LessThanLowRange(float currentInput, EV_BatteryParameterTypesForBMS BatteryParametersName) {
    bool MinThresholdCheck = 0;
    if(currentInput < parameterInfo[BatteryParametersName].minimumThreshold){
	    MinThresholdCheck=1;
	    printOnDisplayLimitBreached(parameterInfo[BatteryParametersName].parameterName,DisplayMessageBasedOnLanguageChosen.messageLimitBreached[1],TestCaseCounter);
    }
    return MinThresholdCheck;
}

bool isBatteryParameter_MoreThanHighRange(float currentInput, EV_BatteryParameterTypesForBMS BatteryParametersName) {
    bool MaxThresholdCheck = 0;
    if(currentInput > parameterInfo[BatteryParametersName].maximumThreshold){
	    MaxThresholdCheck=1;
	    printOnDisplayLimitBreached(parameterInfo[BatteryParametersName].parameterName,DisplayMessageBasedOnLanguageChosen.messageLimitBreached[2],TestCaseCounter);
    }
    return MaxThresholdCheck;
}

bool isBatteryParameter_InLowerToleranceLimitRange(float currentInput, EV_BatteryParameterTypesForBMS BatteryParametersName, float lowerToleranceLimit){
	bool MinToleranceLimitCheck = 0;
	if(currentInput<=lowerToleranceLimit){
		MinToleranceLimitCheck=1;
		printOnDisplayToleranceLimitApproached(parameterInfo[BatteryParametersName].parameterName,DisplayMessageBasedOnLanguageChosen.messageToleranceLimitApproached[1],TestCaseCounter);
	}
	return MinToleranceLimitCheck;
}

bool isBatteryParameter_InHigherToleranceLimitRange(float currentInput, EV_BatteryParameterTypesForBMS BatteryParametersName, float higherToleranceLimit){
	bool MaxToleranceLimitCheck = 0;
	if(currentInput>=higherToleranceLimit){
		MaxToleranceLimitCheck=1;
		printOnDisplayToleranceLimitApproached(parameterInfo[BatteryParametersName].parameterName,DisplayMessageBasedOnLanguageChosen.messageToleranceLimitApproached[2],TestCaseCounter);
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
void CheckBatteryParameter(EV_BatteryParameterTypesForBMS BatteryParametersName,float testBatteryParameter, bool* BatteryErrorStatus, bool* BatteryWarningStatus){
	*BatteryErrorStatus|=isBatteryParametersWithinNormalRange(BatteryParametersName, testBatteryParameter);
	if(*BatteryErrorStatus == 0)
		*BatteryWarningStatus|=isBatteryParametersWithinToleranceLimit(BatteryParametersName, testBatteryParameter); 
}

void ConvertTempToCelcius(float* TemperatureValue){
	printf("Fahrenheit %f\n",*TemperatureValue);
	*TemperatureValue = ((*TemperatureValue - 32)*5)/9;
	printf("Celcius %f\n",*TemperatureValue);
}
void ConvertTestDataToArray(float* testData[],Struct_BatteryTestData* BatteryTestData){
	if(BatteryTestData.TempUnit == 'f' || BatteryTestData.TempUnit == 'F')
		ConvertTempToCelcius(&BatteryTestData.Temperature);
	testData[0] = BatteryTestData.Temperature;
	testData[1] = BatteryTestData.SOC;
	testData[2] = BatteryTestData.ChargeRate;
}

void BatteryIsOk(Struct_BatteryTestData* testData, bool* BatteryErrorStatus, bool* BatteryWarningStatus ) {
   int counter;	
   float testDataArray[3];
   ConvertTestDataToArray(&testDataArray[],testData)
   for (counter=0;counter<NoOfParameter;counter ++){
	   CheckBatteryParameter(parameterInfo[counter].parameter,testDataArray[counter],BatteryErrorStatus,BatteryWarningStatus);
   } 
}

void TestBatteryIsOk(bool expectedOutput,Struct_BatteryTestData testData){
   TestCaseCounter+=1;
   bool testBatteryStatus;
   bool BatteryErrorStatus = 0;
   bool BatteryWarningStatus = 0;
   BatteryIsOk(&testData,&BatteryErrorStatus,&BatteryWarningStatus); 
   if(BatteryErrorStatus == 1 || BatteryWarningStatus ==1)
	   testBatteryStatus = ALL_NOT_OK;
   else{
	   testBatteryStatus = ALL_OK;
	   printALLOk(TestCaseCounter);
   }
   assert(testBatteryStatus==expectedOutput);
}

void TestBatteryParameterWithinRange(char* BatteryParametersName, bool expectedOutput, float testParameter){
   int ParameterIndex = FetchParameterIndexFromName(BatteryParametersName);
   TestCaseCounter+=1;
   bool BatteryErrorStatus = 0;
   bool BatteryWarningStatus = 0;
   bool testParameterStatus;
	
   CheckBatteryParameter(ParameterIndex,testParameter,&BatteryErrorStatus,&BatteryWarningStatus);
   if(BatteryErrorStatus == 1 || BatteryWarningStatus ==1)
	   testParameterStatus = ALL_NOT_OK;
   else{
	   testParameterStatus = ALL_OK;
	   printALLOk(TestCaseCounter);
   }
   assert(testParameterStatus==expectedOutput);
}

int main() {
  DisplayMessageBasedOnSupportedLanguage();
  PopulateParameterInfo();
		  
  setRangeValues(BatteryParameterName.ParameterName[0],0.0,45.0);
  setRangeValues(BatteryParameterName.ParameterName[1],20.0,80.0);
  setRangeValues(BatteryParameterName.ParameterName[2],0.0,0.8);

Struct_BatteryTestData TestParameters[5]={ 
    { 25, 'c' ,70, 0.7},
    { 50, 'C', 85, 0},
    { 40, 'c', 10, 0.9},
    { 43, 'C', 21, 0.4 },
    { 47, 'f', 22, 0.7}
  };
	
//   Testcase 1
  TestBatteryIsOk(ALL_OK,TestParameters[0]);
	
//   Testcase 2
  TestBatteryIsOk(ALL_NOT_OK,TestParameters[1]);
	
//   Testcase 3
  TestBatteryIsOk(ALL_NOT_OK,TestParameters[2]);
	
//   Testcase 4
  TestBatteryIsOk(ALL_NOT_OK,TestParameters[3]);

//   Testcase 5
  setRangeValues(BatteryParameterName.ParameterName[0],10.0,30.0);
  TestBatteryParameterWithinRange(BatteryParameterName.ParameterName[0],ALL_NOT_OK,40.0);
	
//   Testcase 6
  setRangeValues(BatteryParameterName.ParameterName[1],10.0,70.0);
  TestBatteryParameterWithinRange(BatteryParameterName.ParameterName[1],ALL_OK,40.0);
	
//   Testcase 7
  setRangeValues(BatteryParameterName.ParameterName[2],0.0,0.6);
  TestBatteryParameterWithinRange(BatteryParameterName.ParameterName[2],ALL_NOT_OK,0.8);
	
//   Testcase 8
  setRangeValues(BatteryParameterName.ParameterName[0],40.0,60.0);
  TestBatteryParameterWithinRange(BatteryParameterName.ParameterName[0],ALL_NOT_OK,30.0);
	
//   Testcase 9
  setRangeValues(BatteryParameterName.ParameterName[2],0.0,0.6);
  TestBatteryParameterWithinRange(BatteryParameterName.ParameterName[2],ALL_NOT_OK,0.58);
	
//   Testcase 10
  setRangeValues(BatteryParameterName.ParameterName[1],10.0,50.0);
  TestBatteryParameterWithinRange(BatteryParameterName.ParameterName[1],ALL_NOT_OK,12);
	
	TestBatteryIsOk(ALL_OK,TestParameters[4]);
}
