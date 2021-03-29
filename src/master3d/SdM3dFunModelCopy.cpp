#include "SdM3dFunModelCopy.h"


SdM3dFunModelCopy::SdM3dFunModelCopy() : SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_MODEL, SDM3D_TYPE_VERTEX, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT )
  {
  }



SdM3dModel SdM3dFunModelCopy::toModel() const
  {
  QMatrix4x4 matrix;
  matrix.translate( mParamList[1]->toVertex() );
  matrix.rotate( mParamList[4]->toFloat(), QVector3D(1,0,0) );
  matrix.rotate( mParamList[3]->toFloat(), QVector3D(0,1,0) );
  matrix.rotate( mParamList[2]->toFloat(), QVector3D(0,0,1) );
  return sd3dModelMap( mParamList[0]->toModel(), matrix );
  }
