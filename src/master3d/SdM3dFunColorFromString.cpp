#include "SdM3dFunColorFromString.h"


SdM3dFunColorFromString::SdM3dFunColorFromString() :
  SdM3dFunction( SD_SCRIPT_TYPE_COLOR, SD_SCRIPT_TYPE_STRING )
  {

  }


QColor SdM3dFunColorFromString::toColor() const
  {
  return QColor( mParamList[0]->toString() );
  }
