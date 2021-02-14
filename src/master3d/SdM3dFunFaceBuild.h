#ifndef SDM3DFUNFACEBUILD_H
#define SDM3DFUNFACEBUILD_H

#include "SdM3dFunction.h"

class SdM3dFunFaceBuild : public SdM3dFunction
  {
  public:
    SdM3dFunFaceBuild();

    // SdM3dValue interface
  public:
    virtual SdM3dFace toFace() const override;

    static  SdM3dFace faceBuild( SdM3dRegion region, QColor color );
  };


#endif // SDM3DFUNFACEBUILD_H
