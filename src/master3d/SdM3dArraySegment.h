#ifndef SDM3DARRAYSEGMENT_H
#define SDM3DARRAYSEGMENT_H

#include "SdM3dArray.h"

class SdM3dArraySegment : public SdM3dArray
  {
  public:
    SdM3dArraySegment();

    // SdM3dValue interface
  public:
    virtual char      type() const override;
    virtual SdM3dPath toPath() const override;

    // SdM3dArray interface
  public:
    virtual char typeOfElement() const override;
  };

#endif // SDM3DARRAYSEGMENT_H
