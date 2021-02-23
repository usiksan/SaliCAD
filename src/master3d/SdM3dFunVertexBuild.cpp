#include "SdM3dFunVertexBuild.h"


SdM3dFunVertexBuild::SdM3dFunVertexBuild() :
  SdM3dFunction( SDM3D_TYPE_VERTEX, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT )
  {

  }



QVector3D SdM3dFunVertexBuild::toVertex() const
  {
  return QVector3D( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat() );
  }

