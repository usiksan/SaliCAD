#include "SdM3dFunFaceBuild.h"

static const char paramList[2] = { SDM3D_TYPE_REGION, SDM3D_TYPE_COLOR };

SdM3dFunFaceBuild::SdM3dFunFaceBuild() :
  SdM3dFunction( SDM3D_TYPE_FACE, 2, paramList )
  {

  }



SdM3dFace SdM3dFunFaceBuild::toFace() const
  {
  return faceBuild( mParamList[0]->toRegion(), mParamList[1]->toColor() );
  }



SdM3dFace SdM3dFunFaceBuild::faceBuild(SdM3dRegion region, QColor color)
  {
  SdM3dFace face;
  face.mContour = region;
  face.mColor   = color;
  return face;
  }
