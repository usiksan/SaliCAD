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

  Binary operation for comparing to less of two's float
*/
#ifndef SDSCRIPTVALUEOPBINARYFLOATLESS_H
#define SDSCRIPTVALUEOPBINARYFLOATLESS_H

#include "SdScriptValueOpBinaryFloat.h"

class SdScriptValueOpBinaryFloatLess : public SdScriptValueOpBinaryFloat
  {
  public:
    SdScriptValueOpBinaryFloatLess( SdScriptValue *first, SdScriptValue *second ) : SdScriptValueOpBinaryFloat( first, second ) { }

    // SdM3dValue interface
  public:
    virtual char type() const override { return SD_SCRIPT_TYPE_BOOL; }
    virtual bool toBool() const override { return mFirstOperand->toFloat() < mSecondOperand->toFloat(); }
  };

#endif // SDSCRIPTVALUEOPBINARYFLOATLESS_H
