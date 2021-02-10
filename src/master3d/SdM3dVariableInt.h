#ifndef SDM3DVARIABLEINT_H
#define SDM3DVARIABLEINT_H

#include "SdM3dVariable.h"

class SdM3dVariableInt : public SdM3dVariable
  {
    int mValue;
  public:
    SdM3dVariableInt() : mValue(0) {}

    // SdM3dValue interface
  public:
    virtual float toFloat() const override { return static_cast<float>(mValue); }
    virtual int   toInt() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void assign(SdM3dValuePtr src) override { mValue = src->toInt(); }
  };

#endif // SDM3DVARIABLEINT_H
