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

  Binary operation for substraction of two's float
*/
#ifndef SDSCRIPTVALUEOPBINARYFLOATSUB_H
#define SDSCRIPTVALUEOPBINARYFLOATSUB_H

#include "SdScriptValueOpBinaryFloat.h"

class SdScriptValueOpBinaryFloatSub : public SdScriptValueOpBinaryFloat
  {
  public:
    SdScriptValueOpBinaryFloatSub( SdScriptValue *first, SdScriptValue *second ) : SdScriptValueOpBinaryFloat( first, second ) {}

    // SdM3dValue interface
  public:
    virtual float toFloat() const override { return mFirstOperand->toFloat() - mSecondOperand->toFloat(); }
  };

#endif // SDSCRIPTVALUEOPBINARYFLOATSUB_H
