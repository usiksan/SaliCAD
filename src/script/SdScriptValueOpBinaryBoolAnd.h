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

  Executes binary AND operation with two bools
*/
#ifndef SDSCRIPTVALUEOPBINARYBOOLAND_H
#define SDSCRIPTVALUEOPBINARYBOOLAND_H

#include "SdScriptValueOpBinaryBool.h"

class SdScriptValueOpBinaryBoolAnd : public SdScriptValueOpBinaryBool
  {
  public:
    SdScriptValueOpBinaryBoolAnd( SdScriptValue *first, SdScriptValue *second ) : SdScriptValueOpBinaryBool( first, second ) {}

    // SdM3dValue interface
  public:
    //!
    //! \brief toBool Convert object to bool value
    //! \return       bool value
    //!
    virtual bool toBool() const override { return mFirstOperand->toBool() && mSecondOperand->toBool(); }
  };

#endif // SDSCRIPTVALUEOPBINARYBOOLAND_H
