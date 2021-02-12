#ifndef SDM3DBINARYFLOAT_H
#define SDM3DBINARYFLOAT_H

#include "SdM3dBinary.h"

class SdM3dBinaryFloat : public SdM3dBinary
  {
  public:
    SdM3dBinaryFloat( SdM3dValue *first, SdM3dValue *second ) : SdM3dBinary( first, second ) {}

    // SdM3dValue interface
  public:
    virtual char  type() const override { return SDM3D_TYPE_FLOAT; }
  };

#endif // SDM3DBINARYFLOAT_H
