#include "SdM3dFunTransferRegion.h"

SdM3dFunTransferRegion::SdM3dFunTransferRegion()
  {

  }


char SdM3dFunTransferRegion::type() const
  {
  }

SdM3dRegion SdM3dFunTransferRegion::toRegion() const
  {
  }

int SdM3dFunTransferRegion::paramCount() const
  {
  }

char SdM3dFunTransferRegion::paramType(int index) const
  {
  }

SdM3dRegion SdM3dFunTransferRegion::transfer(SdM3dRegion r, QVector3D vertex)
  {
  SdM3dRegion region;
  for( const auto v : r )
    region.append( v + vertex );
  return region;
  }
