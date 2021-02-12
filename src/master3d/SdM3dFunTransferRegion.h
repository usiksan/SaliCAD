#ifndef SDM3DFUNTRANSFERREGION_H
#define SDM3DFUNTRANSFERREGION_H

#include "SdM3dFunction.h"

class SdM3dFunTransferRegion : public SdM3dFunction
  {
  public:
    SdM3dFunTransferRegion();

    // SdM3dValue interface
  public:
    virtual SdM3dRegion toRegion() const override;

    static  SdM3dRegion transfer( SdM3dRegion r, QVector3D vertex );
  };

#endif // SDM3DFUNTRANSFERREGION_H
