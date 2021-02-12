#ifndef SDM3DUNARYBOOLNOT_H
#define SDM3DUNARYBOOLNOT_H

#include "SdM3dUnary.h"

class SdM3dUnaryBoolNot : public SdM3dUnary
  {
  public:
    SdM3dUnaryBoolNot( SdM3dValuePtr op ) : SdM3dUnary( op ) {}

    // SdM3dValue interface
  public:
    virtual char type() const override { return SDM3D_TYPE_BOOL; }
    virtual bool toBool() const override { return !mOperand->toBool(); }
  };

#endif // SDM3DUNARYBOOLNOT_H
