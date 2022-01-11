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

  Executes comparing of two's float values
*/
#ifndef SDSCRIPTVALUEOPBINARYBOOLFLOATLESS_H
#define SDSCRIPTVALUEOPBINARYBOOLFLOATLESS_H

#include "SdScriptValueOpBinaryBool.h"

class SdScriptValueOpBinaryBoolFloatLess : public SdScriptValueOpBinaryBool
  {
  public:
    SdScriptValueOpBinaryBoolFloatLess( SdScriptValue *first, SdScriptValue *second ) : SdScriptValueOpBinaryBool( first, second ) {}

    // SdM3dValue interface
  public:
    //!
    //! \brief toBool Convert object to bool value
    //! \return       bool value
    //!
    virtual bool toBool() const override { return mFirstOperand->toFloat() < mSecondOperand->toFloat(); }
  };

#endif // SDSCRIPTVALUEOPBINARYBOOLFLOATLESS_H
