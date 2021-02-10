#ifndef SDM3DFUNBUILDRECT_H
#define SDM3DFUNBUILDRECT_H

#include "SdM3dFunction.h"

class SdM3dFunBuildRect : public SdM3dFunction
  {
  public:
    SdM3dFunBuildRect();

    // SdM3dValue interface
  public:
    virtual char type() const override;
    virtual SdM3dRegion toRegion() const override;

    // SdM3dFunction interface
  public:
    virtual int paramCount() const override;
    virtual char paramType(int index) const override;

    static SdM3dRegion rectXY( float w, float h );
  };

#endif // SDM3DFUNBUILDRECT_H
