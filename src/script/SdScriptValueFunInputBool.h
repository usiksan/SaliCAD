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

  The function builds and binds input field for bool
*/
#ifndef SDSCRIPTVALUEFUNINPUTBOOL_H
#define SDSCRIPTVALUEFUNINPUTBOOL_H

#include "SdScriptValueFunInput.h"

class SdScriptValueFunInputBool : public SdScriptValueFunInput
  {
  public:
    SdScriptValueFunInputBool(QTableWidget *tableWidget);

    // SdScriptValue interface
  public:
    //!
    //! \brief toBool Convert object to bool value
    //! \return       bool value
    //!
    virtual bool toBool() const override;
  };

#endif // SDSCRIPTVALUEFUNINPUTBOOL_H
