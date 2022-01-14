#ifndef SDM3DVARIABLEBOOL_H
#define SDM3DVARIABLEBOOL_H

#include "SdScriptValueVariable.h"

class SdM3dVariableBool : public SdScriptValueVariable
  {
    bool mValue;
  public:
    SdM3dVariableBool() : mValue(false) {}



    // SdM3dValue interface
  public:
    virtual char type() const override { return SD_SCRIPT_TYPE_BOOL; }
    virtual bool toBool() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void assign(SdM3dValuePtr src) override { mValue = src->toBool(); }

    // SdM3dValue interface
  public:
  };


#endif // SDM3DVARIABLEBOOL_H
