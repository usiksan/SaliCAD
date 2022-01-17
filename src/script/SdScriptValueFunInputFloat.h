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

  The function builds and binds input field for float
*/
#ifndef SDSCRIPTVALUEFUNINPUTFLOAT_H
#define SDSCRIPTVALUEFUNINPUTFLOAT_H

#include "SdScriptValueFunInput.h"

class SdScriptValueFunInputFloat : public SdScriptValueFunInput
  {
  public:
    SdScriptValueFunInputFloat(QTableWidget *tableWidget);

    // SdM3dValue interface
  public:
    //!
    //! \brief toFloat Convert object ot float value
    //! \return        float value
    //!
    virtual float toFloat() const override;
  };

#endif // SDSCRIPTVALUEFUNINPUTFLOAT_H
