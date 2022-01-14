#ifndef SDM3DVARIABLEFACE_H
#define SDM3DVARIABLEFACE_H

#include "SdScriptValueVariable.h"

class SdM3dVariableFace : public SdScriptValueVariable
  {
    SdScriptVal3dFace mValue;
  public:
    SdM3dVariableFace() : mValue() {}



    // SdM3dValue interface
  public:
    virtual char      type() const override { return SD_SCRIPT_TYPE_FACE; }
    virtual SdScriptVal3dFace toFace() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void      assign(SdM3dValuePtr src) override { mValue = src->toFace(); }
  };

#endif // SDM3DVARIABLEFACE_H
