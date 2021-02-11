#include "SdM3dFunBuildRect.h"

SdM3dFunBuildRect::SdM3dFunBuildRect() :
  SdM3dFunction( SDM3D_TYPE_REGION, 2, { SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT } )
  {

  }



SdM3dRegion SdM3dFunBuildRect::toRegion() const
  {
  return rectXY( mParamList[0]->toFloat(), mParamList[1]->toFloat() );
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
