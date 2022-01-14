#ifndef SDM3DVARIABLESTRING_H
#define SDM3DVARIABLESTRING_H

#include "SdScriptValueVariable.h"

class SdM3dVariableString : public SdScriptValueVariable
  {
    QString mValue;
  public:
    SdM3dVariableString() {}

    // SdM3dValue interface
  public:
    virtual char    type() const override { return SD_SCRIPT_TYPE_STRING; }
    virtual QString toString() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void assign(SdM3dValuePtr src) override { mValue = src->toString(); }
  };

#endif // SDM3DVARIABLESTRING_H
