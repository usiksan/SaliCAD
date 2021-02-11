#ifndef SDM3DARRAYFACE_H
#define SDM3DARRAYFACE_H

#include "SdM3dArray.h"

class SdM3dArrayFace : public SdM3dArray
  {
  public:
    SdM3dArrayFace();

    // SdM3dValue interface
  public:
    virtual char       type() const override;
    virtual SdM3dModel toModel() const override;

    // SdM3dArray interface
  public:
    virtual char       typeOfElement() const override;
  };

#endif // SDM3DARRAYFACE_H
