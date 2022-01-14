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

  Embedded function.

  Build color structure from string description
*/
#ifndef SDSCRIPTVALUEFUNCOLORFROMSTRING_H
#define SDSCRIPTVALUEFUNCOLORFROMSTRING_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunColorFromString : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunColorFromString();

    // SdM3dValue interface
  public:
    //!
    //! \brief toColor Convert object to color
    //! \return        Color
    //!
    virtual QColor toColor() const override;
  };

#endif // SDSCRIPTVALUEFUNCOLORFROMSTRING_H
