/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds box model from its size
*/
#ifndef SDM3DFUNMODELBOX_H
#define SDM3DFUNMODELBOX_H

#include "SdM3dFunction.h"

class SdM3dFunModelBox : public SdM3dFunction
  {
  public:
    SdM3dFunModelBox();

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override;

    //!
    //! \brief modelBox Builds box model from its size
    //! \param lenght   Lenght of box (x)
    //! \param width    Width of box (y)
    //! \param height   Height of box (z)
    //! \param color    Color faces of box
    //! \return         Box model
    //!
    static  SdM3dModel modelBox(float lenght, float width, float height, QColor color);
  };

#endif // SDM3DFUNMODELBOX_H
