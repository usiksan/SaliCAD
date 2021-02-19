#ifndef SDM3DFUNREGIONRECT_H
#define SDM3DFUNREGIONRECT_H

#include "SdM3dFunction.h"

class SdM3dFunRegionRect : public SdM3dFunction
  {
  public:
    SdM3dFunRegionRect();

    // SdM3dValue interface
  public:
    virtual SdM3dRegion toRegion() const override;

    static  SdM3dRegion regionRect( float w, float h );
  };

#endif // SDM3DFUNREGIONRECT_H
