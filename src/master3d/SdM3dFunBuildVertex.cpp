#include "SdM3dFunBuildVertex.h"


SdM3dFunBuildVertex::SdM3dFunBuildVertex() :
  SdM3dFunction( SDM3D_TYPE_VERTEX, 3, { SDM3D_TYPE_FLOAT,
                                         SDM3D_TYPE_FLOAT,
                                         SDM3D_TYPE_FLOAT } )
  {

  }



QVector3D SdM3dFunBuildVertex::toVertex() const
  {
  return QVector3D( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat() );
  }

