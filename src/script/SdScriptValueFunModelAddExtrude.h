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

  The function add extrudes the model from the top side of source model in the direction of the normal vector
  with shift amount
*/
#ifndef SDSCRIPTVALUEFUNMODELADDEXTRUDE_H
#define SDSCRIPTVALUEFUNMODELADDEXTRUDE_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunModelAddExtrude : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunModelAddExtrude();

    // SdM3dValue interface
  public:
    //!
    //! \brief toModel Convert object to 3d model
    //! \return        3d model
    //!
    virtual SdScriptVal3dModel toModel() const override;

    //!
    //! \brief modelAddExtrude The function add extrudes the model from the top side of source model in the
    //!                        direction of the normal vector with shift amount
    //! \param src             Source model to add extrusion
    //! \param shift           Shift amount
    //! \param color           Faces color
    //! \return                Model with added extrusion
    //!
    static  SdScriptVal3dModel modelAddExtrude( SdScriptVal3dModel src, float shift, QColor color );
  };

#endif // SDSCRIPTVALUEFUNMODELADDEXTRUDE_H
