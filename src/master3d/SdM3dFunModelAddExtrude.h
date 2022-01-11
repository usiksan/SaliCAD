/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function add extrudes the model from the top side of source model in the direction of the normal vector
  with shift amount
*/
#ifndef SDM3DFUNMODELADDEXTRUDE_H
#define SDM3DFUNMODELADDEXTRUDE_H

#include "SdM3dFunction.h"

class SdM3dFunModelAddExtrude : public SdM3dFunction
  {
  public:
    SdM3dFunModelAddExtrude();

    // SdM3dValue interface
  public:
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

#endif // SDM3DFUNMODELADDEXTRUDE_H
