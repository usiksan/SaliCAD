#include "SdM3dArrayVertex.h"

SdM3dArrayVertex::SdM3dArrayVertex()
  {

  }


char SdM3dArrayVertex::type() const
  {
  return SDM3D_TYPE_REGION;
  }

SdM3dRegion SdM3dArrayVertex::toRegion() const
  {
  SdM3dRegion region;
  for( auto const ptr : mArray )
    region.append( ptr->toVertex() );
  return region;
  }

char SdM3dArrayVertex::typeOfElement() const
  {
  return SDM3D_TYPE_VERTEX;
  }
