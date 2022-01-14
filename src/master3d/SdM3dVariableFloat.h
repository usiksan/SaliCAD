#ifndef SDM3DVARIABLEFLOAT_H
#define SDM3DVARIABLEFLOAT_H

#include "SdScriptValueVariable.h"

class SdM3dVariableFloat : public SdScriptValueVariable
  {
    float mValue;
  public:
    SdM3dVariableFloat() : mValue(0.0) {}

    // SdM3dValue interface
  public:
    virtual char  type() const override { return SD_SCRIPT_TYPE_FLOAT; }
    virtual float toFloat() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void assign(SdM3dValuePtr src) override { mValue = src->toFloat(); }
  };

#endif // SDM3DVARIABLEFLOAT_H
