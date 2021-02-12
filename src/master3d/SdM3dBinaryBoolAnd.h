#ifndef SDM3DBINARYBOOLAND_H
#define SDM3DBINARYBOOLAND_H

#include "SdM3dBinaryBool.h"

class SdM3dBinaryBoolAnd : public SdM3dBinaryBool
  {
  public:
    SdM3dBinaryBoolAnd( SdM3dValue *first, SdM3dValue *second ) : SdM3dBinaryBool( first, second ) {}

    // SdM3dValue interface
  public:
    virtual bool toBool() const override { return mFirstOperand->toBool() && mSecondOperand->toBool(); }
  };

#endif // SDM3DBINARYBOOLAND_H
