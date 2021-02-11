#ifndef SDM3DFLOAT_H
#define SDM3DFLOAT_H

#include "SdM3dValue.h"

class SdM3dFloat : public SdM3dValue
  {
    float mValue;
  public:
    SdM3dFloat( float val ) : SdM3dValue(), mValue(val) {}



    // SdM3dValue interface
  public:
    virtual char  type() const override { return SDM3D_TYPE_FLOAT; }
    virtual float toFloat() const override { return mValue; }
  };

#endif // SDM3DFLOAT_H
