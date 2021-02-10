#ifndef SDM3DBUILDSEGMENT_H
#define SDM3DBUILDSEGMENT_H

#include "SdM3dBinary.h"

class SdM3dBuildSegment : public SdM3dBinary
  {
  public:
    SdM3dBuildSegment();

    // SdM3dValue interface
  public:
    virtual SdM3dSegment toSegment() const override;
  };

#endif // SDM3DBUILDSEGMENT_H
