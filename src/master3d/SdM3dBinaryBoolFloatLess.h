#ifndef SDM3DBINARYBOOLFLOATLESS_H
#define SDM3DBINARYBOOLFLOATLESS_H

#include "SdM3dBinaryBool.h"

class SdM3dBinaryBoolFloatLess : public SdM3dBinaryBool
  {
  public:
    SdM3dBinaryBoolFloatLess( SdM3dValue *first, SdM3dValue *second ) : SdM3dBinaryBool( first, second ) {}

    // SdM3dValue interface
  public:
    virtual bool toBool() const override { return mFirstOperand->toFloat() < mSecondOperand->toFloat(); }
  };

#endif // SDM3DBINARYBOOLFLOATLESS_H
