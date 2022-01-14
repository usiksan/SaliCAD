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

  Base class for all binary operations
*/
#ifndef SDSCRIPTVALUEOPBINARY_H
#define SDSCRIPTVALUEOPBINARY_H

#include "SdScriptValue.h"


class SdScriptValueOpBinary : public SdScriptValue
  {
  protected:
    SdScriptValuePtr mFirstOperand;  //!< First operand of operation
    SdScriptValuePtr mSecondOperand; //!< Second operand of operation
  public:
    SdScriptValueOpBinary( SdScriptValue *first, SdScriptValue *second ) : mFirstOperand(first), mSecondOperand(second) {}

    ~SdScriptValueOpBinary() {
      delete mFirstOperand;
      delete mSecondOperand;
      }
  };

#endif // SDSCRIPTVALUEOPBINARY_H
