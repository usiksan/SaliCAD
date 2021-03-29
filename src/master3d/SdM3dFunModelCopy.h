#ifndef SDM3DFUNMODELCOPY_H
#define SDM3DFUNMODELCOPY_H

#include "SdM3dFunction.h"

class SdM3dFunModelCopy : public SdM3dFunction
  {
  public:
    SdM3dFunModelCopy();

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override;
  };

#endif // SDM3DFUNMODELCOPY_H
