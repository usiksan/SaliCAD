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

  Executes binary OR operation with two bools
*/
#ifndef SDSCRIPTVALUEOPBINARYBOOLOR_H
#define SDSCRIPTVALUEOPBINARYBOOLOR_H

#include "SdScriptValueOpBinaryBool.h"

class SdScriptValueOpBinaryBoolOr : public SdScriptValueOpBinaryBool
  {
  public:
    SdScriptValueOpBinaryBoolOr( SdScriptValue *first, SdScriptValue *second ) : SdScriptValueOpBinaryBool( first, second ) {}

    // SdM3dValue interface
  public:
    //!
    //! \brief toBool Convert object to bool value
    //! \return       bool value
    //!
    virtual bool toBool() const override { return mFirstOperand->toBool() || mSecondOperand->toBool(); }
  };

#endif // SDSCRIPTVALUEOPBINARYBOOLOR_H
