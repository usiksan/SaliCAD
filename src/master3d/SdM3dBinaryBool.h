#ifndef SDM3DBINARYBOOL_H
#define SDM3DBINARYBOOL_H

#include "SdM3dBinary.h"

class SdM3dBinaryBool : public SdM3dBinary
  {
  public:
    SdM3dBinaryBool( SdM3dValue *first, SdM3dValue *second ) : SdM3dBinary( first, second ) {}

    // SdM3dValue interface
  public:
    virtual char type()   const override { return SDM3D_TYPE_BOOL; }
  };

#endif // SDM3DBINARYBOOL_H
