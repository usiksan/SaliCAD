#ifndef SDM3DFUNMODELWALL_H
#define SDM3DFUNMODELWALL_H

#include "SdM3dFunction.h"

class SdM3dFunModelWall : public SdM3dFunction
  {
  public:
    SdM3dFunModelWall();

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override;

    static  SdM3dModel modelWall( SdM3dRegion region, QVector3D grow, QColor color );
  };

#endif // SDM3DFUNMODELWALL_H
