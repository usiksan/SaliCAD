#ifndef SDM3DVARIABLEMODEL_H
#define SDM3DVARIABLEMODEL_H

#include "SdScriptValueVariable.h"

class SdM3dVariableModel : public SdScriptValueVariable
  {
    SdScriptVal3dModel mValue;
  public:
    SdM3dVariableModel() : mValue() {}



    // SdM3dValue interface
  public:
    virtual char       type() const override { return SD_SCRIPT_TYPE_MODEL; }
    virtual SdScriptVal3dModel toModel() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void       assign(SdM3dValuePtr src) override { mValue = src->toModel(); }
  };

#endif // SDM3DVARIABLEMODEL_H
