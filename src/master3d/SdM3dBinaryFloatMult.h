#ifndef SDM3DBINARYFLOATMULT_H
#define SDM3DBINARYFLOATMULT_H

#include "SdM3dBinaryFloat.h"

class SdM3dBinaryFloatMult : public SdM3dBinaryFloat
  {
  public:
    SdM3dBinaryFloatMult( SdScriptValue *first, SdScriptValue *second ) : SdM3dBinaryFloat( first, second ) {}

    // SdM3dValue interface
  public:
    virtual float toFloat() const override { return mFirstOperand->toFloat() * mSecondOperand->toFloat(); }
  };

#endif // SDM3DBINARYFLOATMULT_H
