#include "SdM3dFunColorFromString.h"


SdM3dFunColorFromString::SdM3dFunColorFromString() :
  SdM3dFunction( SDM3D_TYPE_COLOR, SDM3D_TYPE_STRING )
  {

  }


QColor SdM3dFunColorFromString::toColor() const
  {
  return QColor( mParamList[0]->toString() );
  }
