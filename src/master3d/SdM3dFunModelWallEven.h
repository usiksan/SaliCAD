#ifndef SDM3DFUNMODELWALLEVEN_H
#define SDM3DFUNMODELWALLEVEN_H

#include "SdM3dFunction.h"

class SdM3dFunModelWallEven : public SdM3dFunction
  {
  public:
    SdM3dFunModelWallEven();

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override;

    static  SdM3dModel modelWallEven( SdM3dRegion regionPair, QVector3D grow, QColor color );
  };

#endif // SDM3DFUNMODELWALLEVEN_H
