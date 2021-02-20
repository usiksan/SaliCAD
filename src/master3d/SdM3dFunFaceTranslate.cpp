#include "SdM3dFunFaceTranslate.h"
#include "SdM3dFunRegionTranslate.h"

SdM3dFunFaceTranslate::SdM3dFunFaceTranslate() :
  SdM3dFunction( SDM3D_TYPE_FACE, SDM3D_TYPE_FACE, SDM3D_TYPE_VERTEX )
  {

  }




SdM3dFace SdM3dFunFaceTranslate::toFace() const
  {
  return faceTranslate( mParamList[0]->toFace(), mParamList[1]->toVertex() );
  }




SdM3dFace SdM3dFunFaceTranslate::faceTranslate(SdM3dFace face, QVector3D offset)
  {
  SdM3dFace result;
  result.mColor = face.mColor;
  result.mContour = SdM3dFunRegionTranslate::regionTranslate( face.mContour, offset );
  return result;
  }
