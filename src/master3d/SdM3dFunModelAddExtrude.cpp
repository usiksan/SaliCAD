/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function add extrudes the model from the top side of source model in the direction of the vector
*/
#include "SdM3dFunModelAddExtrude.h"
#include "SdM3dFunModelWall.h"
#include "SdM3dFunRegionShift.h"

SdM3dFunModelAddExtrude::SdM3dFunModelAddExtrude() :
  SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_MODEL, SDM3D_TYPE_FLOAT, SDM3D_TYPE_COLOR )
  {

  }


SdM3dModel SdM3dFunModelAddExtrude::toModel() const
  {
  return modelAddExtrude( mParamList[0]->toModel(), mParamList[1]->toFloat(), mParamList[2]->toColor() );
  }



//!
//! \brief modelAddExtrude The function add extrudes the model from the top side of source model in the
//!                        direction of the normal vector with shift amount
//! \param src             Source model to add extrusion
//! \param shift           Shift amount
//! \param color           Faces color
//! \return                Model with added extrusion
//!
SdM3dModel SdM3dFunModelAddExtrude::modelAddExtrude(SdM3dModel src, float shift, QColor color)
  {
  SdM3dModel md(src);

  //Remove bottom side
  SdM3dFace bot = md.takeLast();

  SdM3dFace top( sd3dRegionShift( bot.mContour, shift ), color );

  //Side walls
  md.append( sd3dModelWalls( bot.mContour, top.mContour, color, true ) );

  //Top side
  md.append( top );

  return md;
  }
