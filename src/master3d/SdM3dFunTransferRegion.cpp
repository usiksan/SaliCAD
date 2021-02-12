#include "SdM3dFunTransferRegion.h"

SdM3dFunTransferRegion::SdM3dFunTransferRegion() :
  SdM3dFunction( SDM3D_TYPE_REGION, 2, { SDM3D_TYPE_REGION, SDM3D_TYPE_VERTEX } )
  {

  }



SdM3dRegion SdM3dFunTransferRegion::toRegion() const
  {
  return transfer( mParamList[0]->toRegion(), mParamList[1]->toVertex() );
  }



SdM3dRegion SdM3dFunTransferRegion::transfer(SdM3dRegion r, QVector3D vertex)
  {
  SdM3dRegion region;
  for( const auto v : r )
    region.append( v + vertex );
  return region;
  }
