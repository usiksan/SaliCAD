#ifndef SDM3DBINARYFLOATSUB_H
#define SDM3DBINARYFLOATSUB_H

#include "SdM3dBinaryFloat.h"

class SdM3dBinaryFloatSub : public SdM3dBinaryFloat
  {
  public:
    SdM3dBinaryFloatSub( SdScriptValue *first, SdScriptValue *second ) : SdM3dBinaryFloat( first, second ) {}

    // SdM3dValue interface
  public:
    virtual float toFloat() const override { return mFirstOperand->toFloat() - mSecondOperand->toFloat(); }
  };

#endif // SDM3DBINARYFLOATSUB_H
