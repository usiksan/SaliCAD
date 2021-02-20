#ifndef SDM3DFUNREGIONTRANSLATE_H
#define SDM3DFUNREGIONTRANSLATE_H

#include "SdM3dFunction.h"

class SdM3dFunRegionTranslate : public SdM3dFunction
  {
  public:
    SdM3dFunRegionTranslate();

    // SdM3dValue interface
  public:
    virtual SdM3dRegion toRegion() const override;

    static  SdM3dRegion regionTranslate( SdM3dRegion r, QVector3D vertex );
  };

#endif // SDM3DFUNREGIONTRANSLATE_H
