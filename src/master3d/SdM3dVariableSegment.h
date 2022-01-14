#ifndef SDM3DVARIABLESEGMENT_H
#define SDM3DVARIABLESEGMENT_H

#include "SdScriptValueVariable.h"

class SdM3dVariableSegment : public SdScriptValueVariable
  {
    SdScriptVal3dSegment mValue;
  public:
    SdM3dVariableSegment() : mValue() {}



    // SdM3dValue interface
  public:
    virtual char         type() const override { return SD_SCRIPT_TYPE_SEGMENT; }
    virtual SdScriptVal3dSegment toSegment() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void         assign(SdM3dValuePtr src) override { mValue = src->toSegment(); }
  };

#endif // SDM3DVARIABLESEGMENT_H
