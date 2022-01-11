#ifndef SDM3DBINARYFLOATLESS_H
#define SDM3DBINARYFLOATLESS_H

#include "SdM3dBinaryFloat.h"

class SdM3dBinaryFloatLess : public SdM3dBinaryFloat
  {
  public:
    SdM3dBinaryFloatLess( SdScriptValue *first, SdScriptValue *second ) : SdM3dBinaryFloat( first, second ) { }

    // SdM3dValue interface
  public:
    virtual char type() const override { return SD_SCRIPT_TYPE_BOOL; }
    virtual bool toBool() const override { return mFirstOperand->toFloat() < mSecondOperand->toFloat(); }
  };

#endif // SDM3DBINARYFLOATLESS_H
