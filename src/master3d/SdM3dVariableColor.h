#ifndef SDM3DVARIABLECOLOR_H
#define SDM3DVARIABLECOLOR_H

#include "SdM3dVariable.h"

class SdM3dVariableColor : public SdM3dVariable
  {
    QColor mValue;
  public:
    SdM3dVariableColor() : mValue("black") {}



    // SdM3dValue interface
  public:
    virtual char   type() const override { return SD_SCRIPT_TYPE_COLOR; }
    virtual QColor toColor() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void assign(SdM3dValuePtr src) override { mValue = src->toColor(); }
  };

#endif // SDM3DVARIABLECOLOR_H
