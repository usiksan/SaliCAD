#ifndef SDM3DOPERATION_H
#define SDM3DOPERATION_H

#include "SdM3dValue.h"

#define SDM3D_MAX_OPERAND 8

class SdM3dOperation : public SdM3dValue
  {
  protected:
    SdM3dValuePtr mOperandList[SDM3D_MAX_OPERAND];
  public:
    SdM3dOperation();
    ~SdM3dOperation();
  };

#endif // SDM3DOPERATION_H
