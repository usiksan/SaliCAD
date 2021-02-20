#ifndef SDM3DFUNMODELEXTRUDE_H
#define SDM3DFUNMODELEXTRUDE_H

#include "SdM3dFunction.h"

class SdM3dFunModelExtrude : public SdM3dFunction
  {
  public:
    SdM3dFunModelExtrude();

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override;

    static  SdM3dModel modelExtrude(SdM3dRegion region, QVector3D vector, QColor color);
  };

#endif // SDM3DFUNMODELEXTRUDE_H
