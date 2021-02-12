#ifndef SDM3DUNARYBOOLNEG_H
#define SDM3DUNARYBOOLNEG_H

#include "SdM3dUnary.h"

class SdM3dUnaryBoolNeg : public SdM3dUnary
  {
  public:
    SdM3dUnaryBoolNeg( SdM3dValuePtr op ) : SdM3dUnary( op ) {}

    // SdM3dValue interface
  public:
    virtual char type() const override { return SDM3D_TYPE_BOOL; }
    virtual bool toBool() const override { return !mOperand->toBool(); }
  };

#endif // SDM3DUNARYBOOLNEG_H
