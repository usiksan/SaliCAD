/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Script language is C-style hi-level language for programming 3d models, 2d parametric graphics and
  schematic calculations.

  Embedded function.

  The function builds and binds input field for color
*/
#ifndef SDSCRIPTVALUEFUNINPUTCOLOR_H
#define SDSCRIPTVALUEFUNINPUTCOLOR_H

#include "SdScriptValueFunInput.h"

class SdScriptValueFunInputColor : public SdScriptValueFunInput
  {
  public:
    SdScriptValueFunInputColor(QTableWidget *tableWidget);

    // SdM3dValue interface
  public:
    //!
    //! \brief toColor Convert object to color
    //! \return        Color
    //!
    virtual QColor toColor() const override;
  };

#endif // SDSCRIPTVALUEFUNINPUTCOLOR_H
