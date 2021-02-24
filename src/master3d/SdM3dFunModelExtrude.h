/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function extrudes the model from the region in the direction of the vector
*/

#ifndef SDM3DFUNMODELEXTRUDE_H
#define SDM3DFUNMODELEXTRUDE_H

#include "SdM3dFunction.h"

class SdM3dFunModelExtrude : public SdM3dFunction
  {
  public:
    SdM3dFunModelExtrude();

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override;

    //!
    //! \brief modelExtrude Extrudes model from region in the direction of the normal vector with
    //!                     specified shift amount color faces.
    //!                     First face is bottom (begin of vector), last face is top (end of vector),
    //!                     Middle faces are walls.
    //! \param region       Region of bottom of model
    //! \param shift        Shift amount of extrude
    //! \param color        Face model color
    //! \return             Model extruded from region in the direction of the normal vector
    //!
    static  SdM3dModel modelExtrude(SdM3dRegion region, float shift, QColor color);

  };

#endif // SDM3DFUNMODELEXTRUDE_H
