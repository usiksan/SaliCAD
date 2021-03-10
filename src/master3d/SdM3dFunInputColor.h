/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds and binds input field for color
*/
#ifndef SDM3DFUNINPUTCOLOR_H
#define SDM3DFUNINPUTCOLOR_H

#include "SdM3dFunInput.h"

class SdM3dFunInputColor : public SdM3dFunInput
  {
  public:
    SdM3dFunInputColor(QTableWidget *tableWidget);

    // SdM3dValue interface
  public:
    virtual QColor toColor() const override;
  };

#endif // SDM3DFUNINPUTCOLOR_H
