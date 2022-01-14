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

  Base class for all variables
*/
#ifndef SDSCRIPTVALUEVARIABLE_H
#define SDSCRIPTVALUEVARIABLE_H

#include "SdScriptValue.h"

class SdScriptValueVariable : public SdScriptValue
  {
  public:
    SdScriptValueVariable() : SdScriptValue() {}

    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void assign( SdScriptValuePtr src ) = 0;
  };

using SdScriptValueVariablePtr = SdScriptValueVariable*;

#endif // SDSCRIPTVALUEVARIABLE_H
