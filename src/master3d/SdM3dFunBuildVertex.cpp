#include "SdM3dFunBuildVertex.h"

SdM3dFunBuildVertex::SdM3dFunBuildVertex()
  {

  }


char SdM3dFunBuildVertex::type() const
  {
  return SDM3D_TYPE_VERTEX;
  }

QVector3D SdM3dFunBuildVertex::toVertex() const
  {
  return QVector3D( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat() );
  }

int SdM3dFunBuildVertex::paramCount() const
  {
  return 3;
  }

char SdM3dFunBuildVertex::paramType(int index) const
  {
  static const char pt[] = {
    SDM3D_TYPE_FLOAT,
    SDM3D_TYPE_FLOAT,
    SDM3D_TYPE_FLOAT
  };
  return pt[index];
  }
