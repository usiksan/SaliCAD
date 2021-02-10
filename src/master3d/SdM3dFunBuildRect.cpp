#include "SdM3dFunBuildRect.h"

SdM3dFunBuildRect::SdM3dFunBuildRect()
  {

  }


char SdM3dFunBuildRect::type() const
  {
  }

SdM3dRegion SdM3dFunBuildRect::toRegion() const
  {
  return rectXY( mParamList[0]->toFloat(), mParamList[1]->toFloat() );
  }

int SdM3dFunBuildRect::paramCount() const
  {
  }

char SdM3dFunBuildRect::paramType(int index) const
  {
  }



SdM3dRegion SdM3dFunBuildRect::rectXY(float w, float h)
  {
  SdM3dRegion region;
  region.append( QVector3D(0,0,0) );
  region.append( QVector3D(w,0,0) );
  region.append( QVector3D(w,h,0) );
  region.append( QVector3D(0,h,0) );
  return region;
  }
