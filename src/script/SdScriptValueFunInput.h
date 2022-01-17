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

  This function appends one field to program param table
*/
#ifndef SDSCRIPTVALUEFUNINPUT_H
#define SDSCRIPTVALUEFUNINPUT_H

#include "SdScriptValueFunction.h"

#include <QTableWidget>

class SdScriptValueFunInput : public SdScriptValueFunction
  {
  protected:
    QTableWidget *mTableWidget; //!< Param table widget
    int           mRow;         //!< Row number for this param
  public:
    SdScriptValueFunInput(char resultType, QTableWidget *tableWidget, int itemType );
  };

#endif // SDSCRIPTVALUEFUNINPUT_H
