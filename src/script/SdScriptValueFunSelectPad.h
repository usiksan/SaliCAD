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

  The function used to show master to edit or build pad textual representation and to return it
*/
#ifndef SDSCRIPTVALUEFUNSELECTPAD_H
#define SDSCRIPTVALUEFUNSELECTPAD_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunSelectPad : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunSelectPad() : SdScriptValueFunction( SD_SCRIPT_TYPE_STRING, SD_SCRIPT_TYPE_STRING ) {}

    // SdM3dValue interface
  public:
    //!
    //! \brief toString Convert object to string
    //! \return         String
    //!
    virtual QString toString() const override { return mParamList[0]->toString(); }
  };

#endif // SDSCRIPTVALUEFUNSELECTPAD_H
