#ifndef SDM3DVALUEFLOAT_H
#define SDM3DVALUEFLOAT_H

#include "SdM3dValue.h"

class SdM3dValueFloat : public SdM3dValue
  {
    float mValue;
  public:
    SdM3dValueFloat( float val ) : mValue(val) {}

    // SdM3dValue interface
  public:
  };

#endif // SDM3DVALUEFLOAT_H
