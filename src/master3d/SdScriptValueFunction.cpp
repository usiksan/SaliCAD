#include "SdScriptValueFunction.h"



SdScriptValueFunction::SdScriptValueFunction(char resultType, char paramType0) :
  mResultType(resultType),
  mParamCount(1)
  {
  mParamTypes[0] = paramType0;
  clearParamList();
  }




SdScriptValueFunction::SdScriptValueFunction(char resultType, char paramType0, char paramType1) :
  mResultType(resultType),
  mParamCount(2)
  {
  mParamTypes[0] = paramType0;
  mParamTypes[1] = paramType1;
  clearParamList();
  }




SdScriptValueFunction::SdScriptValueFunction(char resultType, char paramType0, char paramType1, char paramType2) :
  mResultType(resultType),
  mParamCount(3)
  {
  mParamTypes[0] = paramType0;
  mParamTypes[1] = paramType1;
  mParamTypes[2] = paramType2;
  clearParamList();
  }




SdScriptValueFunction::SdScriptValueFunction(char resultType, char paramType0, char paramType1, char paramType2, char paramType3) :
  mResultType(resultType),
  mParamCount(4)
  {
  mParamTypes[0] = paramType0;
  mParamTypes[1] = paramType1;
  mParamTypes[2] = paramType2;
  mParamTypes[3] = paramType3;
  clearParamList();
  }




SdScriptValueFunction::SdScriptValueFunction(char resultType, char paramType0, char paramType1, char paramType2, char paramType3, char paramType4) :
  mResultType(resultType),
  mParamCount(5)
  {
  mParamTypes[0] = paramType0;
  mParamTypes[1] = paramType1;
  mParamTypes[2] = paramType2;
  mParamTypes[3] = paramType3;
  mParamTypes[4] = paramType4;
  clearParamList();
  }




SdScriptValueFunction::SdScriptValueFunction(char resultType, char paramType0, char paramType1, char paramType2, char paramType3, char paramType4, char paramType5) :
  mResultType(resultType),
  mParamCount(6)
  {
  mParamTypes[0] = paramType0;
  mParamTypes[1] = paramType1;
  mParamTypes[2] = paramType2;
  mParamTypes[3] = paramType3;
  mParamTypes[4] = paramType4;
  mParamTypes[5] = paramType5;
  clearParamList();
  }




SdScriptValueFunction::SdScriptValueFunction(char resultType, char paramType0, char paramType1, char paramType2, char paramType3, char paramType4, char paramType5, char paramType6) :
  mResultType(resultType),
  mParamCount(7)
  {
  mParamTypes[0] = paramType0;
  mParamTypes[1] = paramType1;
  mParamTypes[2] = paramType2;
  mParamTypes[3] = paramType3;
  mParamTypes[4] = paramType4;
  mParamTypes[5] = paramType5;
  mParamTypes[6] = paramType6;
  clearParamList();
  }




SdScriptValueFunction::SdScriptValueFunction(char resultType, char paramType0, char paramType1, char paramType2, char paramType3, char paramType4, char paramType5, char paramType6, char paramType7) :
  mResultType(resultType),
  mParamCount(8)
  {
  mParamTypes[0] = paramType0;
  mParamTypes[1] = paramType1;
  mParamTypes[2] = paramType2;
  mParamTypes[3] = paramType3;
  mParamTypes[4] = paramType4;
  mParamTypes[5] = paramType5;
  mParamTypes[6] = paramType6;
  mParamTypes[7] = paramType7;
  clearParamList();
  }



SdScriptValueFunction::~SdScriptValueFunction()
  {
  for( int i = 0; i < SDSCRIPT_FUN_MAX_PARAM; i++ )
    if( mParamList[i] == nullptr ) break;
    else delete mParamList[i];
  }



void SdScriptValueFunction::clearParamList()
  {
  for( int i = 0; i < SDSCRIPT_FUN_MAX_PARAM; i++ )
    mParamList[i] = nullptr;
  }





