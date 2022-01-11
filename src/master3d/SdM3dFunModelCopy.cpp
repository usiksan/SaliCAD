#include "SdM3dFunModelCopy.h"


SdM3dFunModelCopy::SdM3dFunModelCopy() : SdM3dFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT )
  {
  }



SdScriptVal3dModel SdM3dFunModelCopy::toModel() const
  {
  QMatrix4x4 matrix;
  matrix.translate( mParamList[1]->toVertex() );
  matrix.rotate( mParamList[4]->toFloat(), QVector3D(1,0,0) );
  matrix.rotate( mParamList[3]->toFloat(), QVector3D(0,1,0) );
  matrix.rotate( mParamList[2]->toFloat(), QVector3D(0,0,1) );
  return sd3dModelMap( mParamList[0]->toModel(), matrix );
  }
