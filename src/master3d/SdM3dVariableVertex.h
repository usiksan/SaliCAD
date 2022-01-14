#ifndef SDM3DVARIABLEVERTEX_H
#define SDM3DVARIABLEVERTEX_H

#include "SdScriptValueVariable.h"

class SdM3dVariableVertex : public SdScriptValueVariable
  {
    QVector3D mValue;
  public:
    SdM3dVariableVertex() : mValue() {}



    // SdM3dValue interface
  public:
    virtual char      type() const override { return SD_SCRIPT_TYPE_VERTEX; }
    virtual QVector3D toVertex() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void assign(SdM3dValuePtr src) override { mValue = src->toVertex(); }
  };

#endif // SDM3DVARIABLEVERTEX_H
