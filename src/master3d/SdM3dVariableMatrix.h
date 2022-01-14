#ifndef SDM3DVARIABLEMATRIX_H
#define SDM3DVARIABLEMATRIX_H

#include "SdScriptValueVariable.h"

class SdM3dVariableMatrix : public SdScriptValueVariable
  {
    QMatrix4x4 mValue;
  public:
    SdM3dVariableMatrix() : mValue() {}



    // SdM3dValue interface
  public:
    virtual char       type() const override { return SD_SCRIPT_TYPE_MATRIX; }
    virtual QMatrix4x4 toMatrix() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void assign(SdM3dValuePtr src) override { mValue = src->toMatrix(); }
  };

#endif // SDM3DVARIABLEMATRIX_H
