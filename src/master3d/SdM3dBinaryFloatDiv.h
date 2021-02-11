#ifndef SDM3DBINARYFLOATDIV_H
#define SDM3DBINARYFLOATDIV_H

#include "SdM3dBinary.h"

class SdM3dBinaryFloatDiv : public SdM3dBinary
  {
  public:
    SdM3dBinaryFloatDiv( SdM3dValue *first, SdM3dValue *second ) : SdM3dBinary( first, second ) {}

    // SdM3dValue interface
  public:
    virtual char  type() const override { return SDM3D_TYPE_FLOAT; }
    virtual float toFloat() const override {
      float second = mSecondOperand->toFloat();
      if( second == 0 ) second = 0.001;
      return mFirstOperand->toFloat() / second;
      }
  };

#endif // SDM3DBINARYFLOATDIV_H
