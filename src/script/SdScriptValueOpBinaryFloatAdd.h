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

  Binary operation for summaring two's float
*/
#ifndef SDSCRIPTVALUEOPBINARYFLOATADD_H
#define SDSCRIPTVALUEOPBINARYFLOATADD_H

#include "SdScriptValueOpBinaryFloat.h"

class SdScriptValueOpBinaryFloatAdd : public SdScriptValueOpBinaryFloat
  {
  public:
    SdScriptValueOpBinaryFloatAdd( SdScriptValue *first, SdScriptValue *second ) : SdScriptValueOpBinaryFloat( first, second ) {}

    // SdM3dValue interface
  public:
    virtual float toFloat() const override { return mFirstOperand->toFloat() + mSecondOperand->toFloat(); }
  };

#endif // SDSCRIPTVALUEOPBINARYFLOATADD_H
