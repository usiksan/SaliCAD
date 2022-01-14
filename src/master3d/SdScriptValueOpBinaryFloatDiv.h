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

  Binary operation for division two's float
*/
#ifndef SDSCRIPTVALUEOPBINARYFLOATDIV_H
#define SDSCRIPTVALUEOPBINARYFLOATDIV_H

#include "SdScriptValueOpBinaryFloat.h"

class SdScriptValueOpBinaryFloatDiv : public SdScriptValueOpBinaryFloat
  {
  public:
    SdScriptValueOpBinaryFloatDiv( SdScriptValue *first, SdScriptValue *second ) : SdScriptValueOpBinaryFloat( first, second ) {}

    // SdM3dValue interface
  public:
    virtual float toFloat() const override {
      float second = mSecondOperand->toFloat();
      if( second == 0 ) second = 0.001;
      return mFirstOperand->toFloat() / second;
      }
  };

#endif // SDSCRIPTVALUEOPBINARYFLOATDIV_H
