#ifndef SDM3DFLOATSUMM_H
#define SDM3DFLOATSUMM_H

#include "SdM3dBinary.h"

class SdM3dFloatSumm : public SdM3dBinary
  {
  public:
    SdM3dFloatSumm( SdM3dValue *first, SdM3dValue *second ) : SdM3dBinary( first, second ) {}

    // SdM3dValue interface
  public:
    virtual char  type() const override { return SDM3D_TYPE_FLOAT; }
    virtual float toFloat() const override { return mFirstOperand->toFloat() + mSecondOperand->toFloat(); }
  };

#endif // SDM3DFLOATSUMM_H
