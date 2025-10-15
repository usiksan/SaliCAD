#ifndef SDSCANERKICAD_H
#define SDSCANERKICAD_H

#include "../SdScanerMultyline.h"


class SdScanerKiCad : public SdScanerMultyline
  {
  public:
    SdScanerKiCad();

    // SdScaner interface
  public:
    virtual void tokenNext() override;
  };

#endif // SDSCANERKICAD_H
