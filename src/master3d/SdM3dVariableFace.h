#ifndef SDM3DVARIABLEFACE_H
#define SDM3DVARIABLEFACE_H

#include "SdM3dVariable.h"

class SdM3dVariableFace : public SdM3dVariable
  {
    SdM3dFace mValue;
  public:
    SdM3dVariableFace() : mValue() {}



    // SdM3dValue interface
  public:
    virtual SdM3dFace toFace() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void      assign(SdM3dValuePtr src) override { mValue = src->toFace(); }
  };

#endif // SDM3DVARIABLEFACE_H
