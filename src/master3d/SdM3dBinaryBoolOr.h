#ifndef SDM3DBINARYBOOLOR_H
#define SDM3DBINARYBOOLOR_H

#include "SdM3dBinaryBool.h"

class SdM3dBinaryBoolOr : public SdM3dBinaryBool
  {
  public:
    SdM3dBinaryBoolOr( SdM3dValue *first, SdM3dValue *second ) : SdM3dBinaryBool( first, second ) {}

    // SdM3dValue interface
  public:
    virtual bool toBool() const override { return mFirstOperand->toBool() || mSecondOperand->toBool(); }
  };

#endif // SDM3DBINARYBOOLOR_H
