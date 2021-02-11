#ifndef SDM3DARRAYVERTEX_H
#define SDM3DARRAYVERTEX_H

#include "SdM3dArray.h"

class SdM3dArrayVertex : public SdM3dArray
  {
  public:
    SdM3dArrayVertex();

    // SdM3dValue interface
  public:
    virtual char        type() const override;
    virtual SdM3dRegion toRegion() const override;

    // SdM3dArray interface
  public:
    virtual char        typeOfElement() const override;
  };

#endif // SDM3DARRAYVERTEX_H
