#ifndef SDM3DBINARY_H
#define SDM3DBINARY_H

#include "SdM3dValue.h"

class SdM3dBinary : public SdM3dValue
  {
  protected:
    SdM3dValuePtr mFirstOperand;
    SdM3dValuePtr mSecondOperand;
  public:
    SdM3dBinary( SdM3dValue *first, SdM3dValue *second ) : mFirstOperand(first), mSecondOperand(second) {}
    ~SdM3dBinary() {
      delete mFirstOperand;
      delete mSecondOperand;
      }
  };

#endif // SDM3DBINARY_H
