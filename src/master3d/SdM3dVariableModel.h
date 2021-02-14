#ifndef SDM3DVARIABLEMODEL_H
#define SDM3DVARIABLEMODEL_H

#include "SdM3dVariable.h"

class SdM3dVariableModel : public SdM3dVariable
  {
    SdM3dModel mValue;
  public:
    SdM3dVariableModel() : mValue() {}



    // SdM3dValue interface
  public:
    virtual char       type() const override { return SDM3D_TYPE_MODEL; }
    virtual SdM3dModel toModel() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void       assign(SdM3dValuePtr src) override { mValue = src->toModel(); }
  };

#endif // SDM3DVARIABLEMODEL_H
