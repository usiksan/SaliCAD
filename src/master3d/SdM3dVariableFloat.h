#ifndef SDM3DVARIABLEFLOAT_H
#define SDM3DVARIABLEFLOAT_H

#include "SdM3dVariable.h"

class SdM3dVariableFloat : public SdM3dVariable
  {
    float mValue;
  public:
    SdM3dVariableFloat() : mValue(0.0) {}

    // SdM3dValue interface
  public:
    virtual float toFloat() const override { return mValue; }
    virtual int   toInt() const override { return static_cast<int>(mValue); }

    // SdM3dVariable interface
  public:
    virtual void assign(SdM3dValuePtr src) override { mValue = src->toFloat(); }
  };

#endif // SDM3DVARIABLEFLOAT_H
