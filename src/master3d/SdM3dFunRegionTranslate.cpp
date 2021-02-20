#include "SdM3dFunRegionTranslate.h"


SdM3dFunRegionTranslate::SdM3dFunRegionTranslate() :
  SdM3dFunction( SDM3D_TYPE_REGION, SDM3D_TYPE_REGION, SDM3D_TYPE_VERTEX )
  {

  }



SdM3dRegion SdM3dFunRegionTranslate::toRegion() const
  {
  return regionTranslate( mParamList[0]->toRegion(), mParamList[1]->toVertex() );
  }



SdM3dRegion SdM3dFunRegionTranslate::regionTranslate(SdM3dRegion r, QVector3D vertex)
  {
  SdM3dRegion region;
  for( const auto v : r )
    region.append( v + vertex );
  return region;
  }
