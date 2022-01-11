#ifndef SDM3DSTRING_H
#define SDM3DSTRING_H

#include "SdScriptValue.h"

class SdM3dString : public SdScriptValue
  {
    QString mValue;
  public:
    SdM3dString( const QString &val ) : SdScriptValue(), mValue(val) {}

    // SdM3dValue interface
  public:
    virtual char    type() const override { return SD_SCRIPT_TYPE_STRING;  }
    virtual QString toString() const override { return mValue; }
  };

#endif // SDM3DSTRING_H
