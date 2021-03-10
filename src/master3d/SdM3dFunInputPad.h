/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds and binds input field for pad
*/
#ifndef SDM3DFUNINPUTPAD_H
#define SDM3DFUNINPUTPAD_H

#include "SdM3dFunInput.h"

class SdM3dFunInputPad : public SdM3dFunInput
  {
  public:
    SdM3dFunInputPad(QTableWidget *tableWidget);

    // SdM3dValue interface
  public:
    virtual QString toString() const override;
  };

#endif // SDM3DFUNINPUTPAD_H
