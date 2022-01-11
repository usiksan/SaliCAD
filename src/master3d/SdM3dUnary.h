#ifndef SDM3DUNARY_H
#define SDM3DUNARY_H

#include "SdScriptValue.h"

class SdM3dUnary : public SdScriptValue
  {
  protected:
    SdM3dValuePtr mOperand;
  public:
    SdM3dUnary( SdM3dValuePtr op ) : SdScriptValue(), mOperand(op) {}
    ~SdM3dUnary() { delete mOperand; }
  };

#endif // SDM3DUNARY_H
