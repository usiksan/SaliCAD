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
#include "SdM3dFunModelBox.h"
#include "SdM3dFunModelExtrude.h"
#include "SdM3dFunRegionRect.h"

SdM3dFunModelBox::SdM3dFunModelBox() :
  SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_COLOR )
  {

  }



SdM3dModel SdM3dFunModelBox::toModel() const
  {
  return modelBox( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toColor() );
  }




//!
//! \brief modelBox Builds box model from its size
//! \param lenght   Lenght of box (x)
//! \param width    Width of box (y)
//! \param height   Height of box (z)
//! \param color    Color faces of box
//! \return         Box model
//!
SdM3dModel SdM3dFunModelBox::modelBox(float lenght, float width, float height, QColor color )
  {
  return SdM3dFunModelExtrude::modelExtrude( SdM3dFunRegionRect::regionRect( lenght, width ), height, color );
  }
