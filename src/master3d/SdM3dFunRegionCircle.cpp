#include "SdM3dFunRegionCircle.h"

#include <math.h>

SdM3dFunRegionCircle::SdM3dFunRegionCircle() :
  SdM3dFunction( SDM3D_TYPE_REGION, SDM3D_TYPE_FLOAT )
  {
  }




SdM3dRegion SdM3dFunRegionCircle::toRegion() const
  {
  return regionCircle( mParamList[0]->toFloat() );
  }





//!
//! \brief regionCircle This function builds circle region on base radius with center at 0
//! \param radius       Radius of builded circle
//! \return             Circle region on XY plane
//!
SdM3dRegion SdM3dFunRegionCircle::regionCircle(float radius)
  {
  SdM3dRegion region;
  //Build circle with step 10 degree
  for( int angleDegree = 0; angleDegree < 360; angleDegree += 10 ) {
    float angle = angleDegree;
    angle = angle * M_PI / 180.0;
    QVector3D v( sin(angle) * radius, cos(angle) * radius, 0 );
    region.append( v );
    }
  return region;
  }
