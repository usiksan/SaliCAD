#ifndef SDM3DVARIABLEREGION_H
#define SDM3DVARIABLEREGION_H

#include "SdM3dVariable.h"

class SdM3dVariableRegion : public SdM3dVariable
  {
    SdScriptVal3dRegion mValue;
  public:
    SdM3dVariableRegion() : mValue() {}



    // SdM3dValue interface
  public:
    virtual char        type() const override { return SD_SCRIPT_TYPE_REGION; }
    virtual SdScriptVal3dRegion toRegion() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void        assign(SdM3dValuePtr src) override { mValue = src->toRegion(); }
  };

#endif // SDM3DVARIABLEREGION_H
