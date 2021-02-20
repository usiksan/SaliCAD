#ifndef SDM3DFUNCOLORFROMSTRING_H
#define SDM3DFUNCOLORFROMSTRING_H

#include "SdM3dFunction.h"

class SdM3dFunColorFromString : public SdM3dFunction
  {
  public:
    SdM3dFunColorFromString();

    // SdM3dValue interface
  public:
    virtual QColor toColor() const override;
  };

#endif // SDM3DFUNCOLORFROMSTRING_H
