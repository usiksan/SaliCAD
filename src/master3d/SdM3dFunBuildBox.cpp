#include "SdM3dFunBuildBox.h"
#include "SdM3dFunBuildRect.h"
#include "SdM3dFunExtrudeModel.h"

SdM3dFunBuildBox::SdM3dFunBuildBox() :
  SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_COLOR )
  {

  }



SdM3dModel SdM3dFunBuildBox::toModel() const
  {
  return box( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toColor() );
  }


SdM3dModel SdM3dFunBuildBox::box(float w, float h, float z, QColor color )
  {
  return SdM3dFunExtrudeModel::model( SdM3dFunBuildRect::rectXY( w, h ), QVector3D(0,0,z), color );
  }
