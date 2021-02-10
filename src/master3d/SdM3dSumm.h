#ifndef SDM3DSUMM_H
#define SDM3DSUMM_H

#include "SdM3dBinary.h"

class SdM3dSumm : public SdM3dBinary
  {
  public:
    SdM3dSumm();

    // SdM3dValue interface
  public:
    virtual float toFloat() const override;
    virtual int toInt() const override;
  };

#endif // SDM3DSUMM_H
