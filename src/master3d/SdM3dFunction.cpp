#include "SdM3dFunction.h"



SdM3dFunction::SdM3dFunction(char resultType, int paramCount, const char *paramTypes) :
  mResultType(resultType),
  mParamCount(paramCount),
  mParamTypes(paramTypes)
  {
  for( int i = 0; i < SDM3D_MAX_PARAM; i++ )
    mParamList[i] = nullptr;
  }



SdM3dFunction::~SdM3dFunction()
  {
  for( int i = 0; i < SDM3D_MAX_PARAM; i++ )
    if( mParamList[i] == nullptr ) break;
    else delete mParamList[i];
  }





