#ifndef SDM3DVARIABLEPATH_H
#define SDM3DVARIABLEPATH_H

#include "SdScriptValueVariable.h"

class SdM3dVariablePath : public SdScriptValueVariable
  {
    SdScriptVal3dPath mValue;
  public:
    SdM3dVariablePath() : mValue() {}



    // SdM3dValue interface
  public:
    virtual char      type() const override { return SD_SCRIPT_TYPE_PATH; }
    virtual SdScriptVal3dPath toPath() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void      assign(SdM3dValuePtr src) override { mValue = src->toPath(); }
  };

#endif // SDM3DVARIABLEPATH_H
