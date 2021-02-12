#ifndef SDM3DVARIABLEBOOL_H
#define SDM3DVARIABLEBOOL_H

#include "SdM3dVariable.h"

class SdM3dVariableBool : public SdM3dVariable
  {
    bool mValue;
  public:
    SdM3dVariableBool() : mValue(false) {}



    // SdM3dValue interface
  public:
    virtual bool toBool() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void assign(SdM3dValuePtr src) override { mValue = src->toBool(); }
  };

#endif // SDM3DVARIABLEBOOL_H
