#ifndef SDM3DVARIABLEPIN_H
#define SDM3DVARIABLEPIN_H

#include "SdM3dVariable.h"

class SdM3dVariablePin : public SdM3dVariable
  {
    SdM3dPin mValue;
  public:
    SdM3dVariablePin() : mValue() {}



    // SdM3dValue interface
  public:
    virtual SdM3dPin toPin() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void     assign(SdM3dValuePtr src) override { mValue = src->toPin(); }
  };

#endif // SDM3DVARIABLEPIN_H
