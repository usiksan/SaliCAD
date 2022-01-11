#ifndef SDM3DBINARYFLOATDIV_H
#define SDM3DBINARYFLOATDIV_H

#include "SdM3dBinaryFloat.h"

class SdM3dBinaryFloatDiv : public SdM3dBinaryFloat
  {
  public:
    SdM3dBinaryFloatDiv( SdScriptValue *first, SdScriptValue *second ) : SdM3dBinaryFloat( first, second ) {}

    // SdM3dValue interface
  public:
    virtual float toFloat() const override {
      float second = mSecondOperand->toFloat();
      if( second == 0 ) second = 0.001;
      return mFirstOperand->toFloat() / second;
      }
  };

#endif // SDM3DBINARYFLOATDIV_H
