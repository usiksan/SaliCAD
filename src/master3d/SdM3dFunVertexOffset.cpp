#include "SdM3dFunVertexOffset.h"


SdM3dFunVertexOffset::SdM3dFunVertexOffset() :
  SdM3dFunction( SDM3D_TYPE_VERTEX, SDM3D_TYPE_VERTEX, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT )
  {

  }



QVector3D SdM3dFunVertexOffset::toVertex() const
  {
  return mParamList[0]->toVertex() + QVector3D( mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat() );
  }
