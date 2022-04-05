#include "SdScriptValueVariableRef.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdGraphSymImp.h"
#include "objects/SdGraphParam.h"
#include "windows/SdDRowValue.h"

SdScriptValueVariableRef::SdScriptValueVariableRef()
  {

  }

float SdScriptValueVariableRef::toFloat() const
  {
  //We retrive value from visual table
  if( mFunRef == nullptr )
    return 0.0;
  return SdDRowValue::phisToDouble( mFunRef->toString() );
  }
