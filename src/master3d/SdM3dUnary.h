#ifndef SDM3DUNARY_H
#define SDM3DUNARY_H

#include "SdM3dValue.h"

class SdM3dUnary : public SdM3dValue
  {
    SdM3dValuePtr mOperand;
  public:
    SdM3dUnary();
  };

#endif // SDM3DUNARY_H
