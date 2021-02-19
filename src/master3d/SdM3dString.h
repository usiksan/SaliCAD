#ifndef SDM3DSTRING_H
#define SDM3DSTRING_H

#include "SdM3dValue.h"

class SdM3dString : public SdM3dValue
  {
    QString mValue;
  public:
    SdM3dString( const QString &val ) : SdM3dValue(), mValue(val) {}

    // SdM3dValue interface
  public:
    virtual char    type() const override { return SDM3D_TYPE_STRING;  }
    virtual QString toString() const override { return mValue; }
  };

#endif // SDM3DSTRING_H
