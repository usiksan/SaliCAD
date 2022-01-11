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

  Base class for all binary operations result of which is bool
*/
#ifndef SDSCRIPTVALUEOPBINARYBOOL_H
#define SDSCRIPTVALUEOPBINARYBOOL_H

#include "SdScriptValueOpBinary.h"

class SdScriptValueOpBinaryBool : public SdScriptValueOpBinary
  {
  public:
    SdScriptValueOpBinaryBool( SdScriptValue *first, SdScriptValue *second ) : SdScriptValueOpBinary( first, second ) {}

    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char type()   const override { return SD_SCRIPT_TYPE_BOOL; }
  };

#endif // SDSCRIPTVALUEOPBINARYBOOL_H
