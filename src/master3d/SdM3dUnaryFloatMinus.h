#ifndef SDM3DUNARYFLOATMINUS_H
#define SDM3DUNARYFLOATMINUS_H

#include "SdM3dUnary.h"

class SdM3dUnaryFloatMinus : public SdM3dUnary
  {
  public:
    SdM3dUnaryFloatMinus(SdM3dValuePtr op) : SdM3dUnary(op) {}


    // SdM3dValue interface
  public:
    virtual char  type() const override { return SDM3D_TYPE_FLOAT; }
    virtual float toFloat() const override { return -mOperand->toFloat(); }
  };

#endif // SDM3DUNARYFLOATMINUS_H
