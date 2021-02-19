#ifndef SDM3DFUNINPUTFLOAT_H
#define SDM3DFUNINPUTFLOAT_H

#include "SdM3dFunInput.h"

class SdM3dFunInputFloat : public SdM3dFunInput
  {
  public:
    SdM3dFunInputFloat(QTableWidget *tableWidget);

    // SdM3dValue interface
  public:
    virtual float toFloat() const override;
  };

#endif // SDM3DFUNINPUTFLOAT_H
