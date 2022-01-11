#ifndef SDM3DVARIABLE_H
#define SDM3DVARIABLE_H

#include "SdScriptValue.h"

class SdM3dVariable : public SdScriptValue
  {
  public:
    SdM3dVariable() : SdScriptValue() {}

    virtual void assign( SdM3dValuePtr src ) = 0;
  };

using SdM3dVariablePtr = SdM3dVariable*;

#endif // SDM3DVARIABLE_H
