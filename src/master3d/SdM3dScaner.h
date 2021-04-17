#ifndef SDM3DSCANER_H
#define SDM3DSCANER_H

#include "import/SdScanerMultyline.h"

class SdM3dScaner : public SdScanerMultyline
  {
  public:
    SdM3dScaner();

    // SdScaner interface
  public:
    virtual void tokenNext() override;
  };

#endif // SDM3DSCANER_H
