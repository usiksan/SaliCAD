#ifndef SDM3DVARIABLE_H
#define SDM3DVARIABLE_H

#include "SdM3dValue.h"

class SdM3dVariable : public SdM3dValue
  {
  public:
    SdM3dVariable();

    virtual void assign( SdM3dValuePtr src ) = 0;
  };

using SdM3dVariablePtr = SdM3dVariable*;

#endif // SDM3DVARIABLE_H
