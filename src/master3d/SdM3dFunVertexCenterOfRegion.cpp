#include "SdM3dFunVertexCenterOfRegion.h"

SdM3dFunVertexCenterOfRegion::SdM3dFunVertexCenterOfRegion() :
  SdM3dFunction( SDM3D_TYPE_VERTEX, SDM3D_TYPE_REGION )
  {

  }


QVector3D SdM3dFunVertexCenterOfRegion::toVertex() const
  {
  return vertexCenterOfRegion( mParamList[0]->toRegion() );
  }




QVector3D SdM3dFunVertexCenterOfRegion::vertexCenterOfRegion(SdM3dRegion region)
  {
  float x = 0;
  float y = 0;
  float z = 0;
  for( auto const &v : region ) {
    x += v.x();
    y += v.y();
    z += v.z();
    }
  float count = region.count();
  return QVector3D( x / count, y / count, z / count );
  }
