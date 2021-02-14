#ifndef SDM3DVARIABLETEXT_H
#define SDM3DVARIABLETEXT_H

#include "SdM3dVariable.h"

class SdM3dVariableText : public SdM3dVariable
  {
    SdM3dText mValue;
  public:
    SdM3dVariableText() : mValue() {}



    // SdM3dValue interface
  public:
    virtual char      type() const override { return SDM3D_TYPE_TEXT; }
    virtual SdM3dText toText() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void      assign(SdM3dValuePtr src) override { mValue = src->toText(); }
  };


#endif // SDM3DVARIABLETEXT_H
