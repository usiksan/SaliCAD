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
SdM3dRegion SdM3dFunRegionCircle::regionCircle(float radius, float stepDegree , QVector3D offset)
  {
  SdM3dRegion region;
  //Build circle with step 10 degree
  for( float angleDegree = 0; angleDegree < 360.0; angleDegree += stepDegree ) {
    //Convert degree to radians
    float angle = angleDegree * M_PI / 180.0;
    //Build next corner
    QVector3D v( sin(angle) * radius, cos(angle) * radius, 0 );
    //Append corner to region
    region.append( v + offset );
    }
  return region;
  }
