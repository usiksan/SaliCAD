/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds cylinder model from its size
*/
#ifndef SDM3DFUNMODELCYLINDER_H
#define SDM3DFUNMODELCYLINDER_H

#include "SdM3dFunction.h"

class SdM3dFunModelCylinder : public SdM3dFunction
  {
  public:
    SdM3dFunModelCylinder();

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override;

    //!
    //! \brief modelCylinder Builds cylinder model from its size
    //! \param radius        Radius of circle of footing of cylinder
    //! \param height        Height of cylinder
    //! \param color         Color of cylinder
    //! \return              Cylinder model
    //!
    static  SdM3dModel modelCylinder( float radius, float height, QColor color );
  };

#endif // SDM3DFUNMODELCYLINDER_H
