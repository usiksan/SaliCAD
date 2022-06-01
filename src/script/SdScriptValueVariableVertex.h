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

  Variable of vertex
*/
#ifndef SDSCRIPTVALUEVARIABLEVERTEX_H
#define SDSCRIPTVALUEVARIABLEVERTEX_H

#include "SdScriptValueVariable3d.h"

class SdScriptValueVariableVertex : public SdScriptValueVariable3d
  {
  public:
    SdScriptValueVariableVertex() {}



    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char      type() const override { return SD_SCRIPT_TYPE_VERTEX; }

    // SdM3dVariable interface
  public:
  };

#endif // SDSCRIPTVALUEVARIABLEVERTEX_H
