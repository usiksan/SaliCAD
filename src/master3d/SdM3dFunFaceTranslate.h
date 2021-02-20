#ifndef SDM3DFUNFACETRANSLATE_H
#define SDM3DFUNFACETRANSLATE_H

#include "SdM3dFunction.h"

class SdM3dFunFaceTranslate : public SdM3dFunction
  {
  public:
    SdM3dFunFaceTranslate();

    // SdM3dValue interface
  public:
    virtual SdM3dFace toFace() const override;

    static  SdM3dFace faceTranslate( SdM3dFace face, QVector3D offset );
  };

#endif // SDM3DFUNFACETRANSLATE_H
