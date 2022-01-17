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

  The function copy model to other position and orientation
*/
#ifndef SDSCRIPTVALUEFUNMODELCOPY_H
#define SDSCRIPTVALUEFUNMODELCOPY_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunModelCopy : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunModelCopy();

    // SdM3dValue interface
  public:
    //!
    //! \brief toModel Convert object to 3d model
    //! \return        3d model
    //!
    virtual SdScriptVal3dModel toModel() const override;
  };

#endif // SDSCRIPTVALUEFUNMODELCOPY_H
