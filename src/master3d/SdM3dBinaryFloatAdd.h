#ifndef SDM3DBINARYFLOATADD_H
#define SDM3DBINARYFLOATADD_H

#include "SdM3dBinaryFloat.h"

class SdM3dBinaryFloatAdd : public SdM3dBinaryFloat
  {
  public:
    SdM3dBinaryFloatAdd( SdM3dValue *first, SdM3dValue *second ) : SdM3dBinaryFloat( first, second ) {}

    // SdM3dValue interface
  public:
    virtual float toFloat() const override { return mFirstOperand->toFloat() + mSecondOperand->toFloat(); }
  };

#endif // SDM3DBINARYFLOATADD_H
