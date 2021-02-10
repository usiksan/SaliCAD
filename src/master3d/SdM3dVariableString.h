#ifndef SDM3DVARIABLESTRING_H
#define SDM3DVARIABLESTRING_H

#include "SdM3dVariable.h"

class SdM3dVariableString : public SdM3dVariable
  {
    QString mValue;
  public:
    SdM3dVariableString();

    // SdM3dValue interface
  public:
    virtual char    type() const override { return SDM3D_TYPE_STRING; }
    //virtual float   toFloat() const override { return mValue.toFloat(); }
    virtual QString toString() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void assign(SdM3dValuePtr src) override { mValue = src->toString(); }
  };

#endif // SDM3DVARIABLESTRING_H
