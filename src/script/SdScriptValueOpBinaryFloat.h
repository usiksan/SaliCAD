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

  Base class for all float binary operations
*/
#ifndef SDSCRIPTVALUEOPBINARYFLOAT_H
#define SDSCRIPTVALUEOPBINARYFLOAT_H

#include "SdScriptValueOpBinary.h"

class SdScriptValueOpBinaryFloat : public SdScriptValueOpBinary
  {
  public:
    SdScriptValueOpBinaryFloat( SdScriptValue *first, SdScriptValue *second ) : SdScriptValueOpBinary( first, second ) {}

    // SdM3dValue interface
  public:
    virtual char  type() const override { return SD_SCRIPT_TYPE_FLOAT; }
  };

#endif // SDSCRIPTVALUEOPBINARYFLOAT_H
