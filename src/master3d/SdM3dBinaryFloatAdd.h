#ifndef SDM3DBINARYFLOATADD_H
#define SDM3DBINARYFLOATADD_H

#include "SdM3dBinary.h"

class SdM3dBinaryFloatAdd : public SdM3dBinary
  {
  public:
    SdM3dBinaryFloatAdd( SdM3dValue *first, SdM3dValue *second ) : SdM3dBinary( first, second ) {}

    // SdM3dValue interface
  public:
    virtual char  type() const override { return SDM3D_TYPE_FLOAT; }
    virtual float toFloat() const override { return mFirstOperand->toFloat() + mSecondOperand->toFloat(); }
  };

#endif // SDM3DBINARYFLOATADD_H
