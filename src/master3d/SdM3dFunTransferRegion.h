#ifndef SDM3DFUNTRANSFERREGION_H
#define SDM3DFUNTRANSFERREGION_H

#include "SdM3dFunction.h"

class SdM3dFunTransferRegion : public SdM3dFunction
  {
  public:
    SdM3dFunTransferRegion();

    // SdM3dValue interface
  public:
    virtual char type() const override;
    virtual SdM3dRegion toRegion() const override;

    // SdM3dFunction interface
  public:
    virtual int paramCount() const override;
    virtual char paramType(int index) const override;

    static SdM3dRegion transfer( SdM3dRegion r, QVector3D vertex );
  };

#endif // SDM3DFUNTRANSFERREGION_H
