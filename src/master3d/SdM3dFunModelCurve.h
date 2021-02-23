#ifndef SDM3DFUNMODELCURVE_H
#define SDM3DFUNMODELCURVE_H

#include "SdM3dFunction.h"

class SdM3dFunModelCurve : public SdM3dFunction
  {
  public:
    SdM3dFunModelCurve();

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override;

    static  SdM3dModel modelCurve(SdM3dRegion region, QVector3D rotationAxis, float angle , QColor color);
  };

#endif // SDM3DFUNMODELCURVE_H
