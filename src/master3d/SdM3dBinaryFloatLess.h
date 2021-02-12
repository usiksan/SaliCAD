#ifndef SDM3DBINARYFLOATLESS_H
#define SDM3DBINARYFLOATLESS_H

#include "SdM3dBinary.h"

class SdM3dBinaryFloatLess : public SdM3dBinary
  {
  public:
    SdM3dBinaryFloatLess( SdM3dValue *first, SdM3dValue *second ) : SdM3dBinary( first, second ) {}

    // SdM3dValue interface
  public:
    virtual char type()   const override { return SDM3D_TYPE_BOOL; }
    virtual bool toBool() const override { return mFirstOperand->toFloat() < mSecondOperand->toFloat(); }
  };

#endif // SDM3DBINARYFLOATLESS_H
