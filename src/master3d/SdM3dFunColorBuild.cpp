#include "SdM3dFunColorBuild.h"


SdM3dFunColorBuild::SdM3dFunColorBuild() :
  SdM3dFunction( SDM3D_TYPE_COLOR, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT )
  {
  }



QColor SdM3dFunColorBuild::toColor() const
  {
  return colorBuild( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat() );
  }




QColor SdM3dFunColorBuild::colorBuild(float r, float g, float b, float a)
  {
  QColor color;
  color.setRgbF( r, g, b, a );
  return color;
  }
