#include "SdM3dArraySegment.h"

SdM3dArraySegment::SdM3dArraySegment()
  {

  }


char SdM3dArraySegment::type() const
  {
  return SDM3D_TYPE_PATH;
  }

SdM3dPath SdM3dArraySegment::toPath() const
  {
  SdM3dPath path;
  for( auto ptr : mArray )
    path.append( ptr->toSegment() );
  return path;
  }

char SdM3dArraySegment::typeOfElement() const
  {
  return SDM3D_TYPE_SEGMENT;
  }
