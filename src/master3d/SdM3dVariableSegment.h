#ifndef SDM3DVARIABLESEGMENT_H
#define SDM3DVARIABLESEGMENT_H

#include "SdM3dVariable.h"

class SdM3dVariableSegment : public SdM3dVariable
  {
    SdM3dSegment mValue;
  public:
    SdM3dVariableSegment() : mValue() {}



    // SdM3dValue interface
  public:
    virtual char         type() const override { return SDM3D_TYPE_SEGMENT; }
    virtual SdM3dSegment toSegment() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void         assign(SdM3dValuePtr src) override { mValue = src->toSegment(); }
  };

#endif // SDM3DVARIABLESEGMENT_H
