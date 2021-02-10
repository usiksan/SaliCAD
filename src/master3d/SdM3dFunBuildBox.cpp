#include "SdM3dFunBuildBox.h"
#include "SdM3dFunBuildRect.h"
#include "SdM3dFunExtrudeModel.h"

SdM3dFunBuildBox::SdM3dFunBuildBox()
  {

  }


char SdM3dFunBuildBox::type() const
  {
  }

SdM3dModel SdM3dFunBuildBox::toModel() const
  {
  }

int SdM3dFunBuildBox::paramCount() const
  {
  }

char SdM3dFunBuildBox::paramType(int index) const
  {
  }

SdM3dModel SdM3dFunBuildBox::box(float w, float h, float z, QColor color )
  {
  return SdM3dFunExtrudeModel::model( SdM3dFunBuildRect::rectXY( w, h ), QVector3D(0,0,z), color );
  }
