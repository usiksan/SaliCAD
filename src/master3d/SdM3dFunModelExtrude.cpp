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
#include "SdM3dFunModelExtrude.h"
#include "SdM3dFunRegionTranslate.h"
#include "SdM3dFunModelWall.h"


SdM3dFunModelExtrude::SdM3dFunModelExtrude() :
  SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_REGION, SDM3D_TYPE_VERTEX, SDM3D_TYPE_COLOR )
  {

  }




SdM3dModel SdM3dFunModelExtrude::toModel() const
  {
  return modelExtrude( mParamList[0]->toRegion(), mParamList[1]->toVertex(), mParamList[2]->toColor() );
  }




//!
//! \brief modelExtrude Extrudes model from region in the direction of the vector with color faces.
//!                     First face is bottom (begin of vector), last face is top (end of vector),
//!                     Middle faces are walls.
//! \param region       Region of bottom of model
//! \param vector       Vector of extrude direction
//! \param color        Face model color
//! \return             Model extruded from region in the direction of the vector
//!
SdM3dModel SdM3dFunModelExtrude::modelExtrude(SdM3dRegion region, QVector3D vector, QColor color )
  {
  SdM3dModel md;
  SdM3dFace bot;
  bot.mContour = region;
  bot.mColor   = color;

  //Bottom side
  md.append( bot );

  SdM3dFace top;
  top.mContour = SdM3dFunRegionTranslate::regionTranslate( region, vector );
  top.mColor   = color;

  //Side walls
  md.append( SdM3dFunModelWall::modelWalls( region, top.mContour, color, true ) );

  //Top side
  md.append( top );

  return md;
  }
