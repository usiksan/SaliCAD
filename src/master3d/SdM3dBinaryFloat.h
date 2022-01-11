#ifndef SDM3DBINARYFLOAT_H
#define SDM3DBINARYFLOAT_H

#include "SdScriptValueOpBinary.h"

class SdM3dBinaryFloat : public SdScriptValueOpBinary
  {
  public:
    SdM3dBinaryFloat( SdScriptValue *first, SdScriptValue *second ) : SdScriptValueOpBinary( first, second ) {}

    // SdM3dValue interface
  public:
    virtual char  type() const override { return SD_SCRIPT_TYPE_FLOAT; }
  };

#endif // SDM3DBINARYFLOAT_H
