#ifndef SDM3DUNARY_H
#define SDM3DUNARY_H

#include "SdM3dValue.h"

class SdM3dUnary : public SdM3dValue
  {
  protected:
    SdM3dValuePtr mOperand;
  public:
    SdM3dUnary( SdM3dValuePtr op ) : SdM3dValue(), mOperand(op) {}
    ~SdM3dUnary() { delete mOperand; }
  };

#endif // SDM3DUNARY_H
