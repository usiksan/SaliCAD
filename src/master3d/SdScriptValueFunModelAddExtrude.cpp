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

  The function add extrudes the model from the top side of source model in the direction of the vector
*/
#include "SdScriptValueFunModelAddExtrude.h"
#include "SdScriptValueFunModelWall.h"
#include "SdScriptValueFunRegionShift.h"

SdScriptValueFunModelAddExtrude::SdScriptValueFunModelAddExtrude() :
  SdScriptValueFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_COLOR )
  {

  }


SdScriptVal3dModel SdScriptValueFunModelAddExtrude::toModel() const
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
SdScriptVal3dModel SdScriptValueFunModelAddExtrude::modelAddExtrude(SdScriptVal3dModel src, float shift, QColor color)
  {
  SdScriptVal3dModel md(src);

  //Remove bottom side
  SdScriptVal3dFace bot = md.takeLast();

  SdScriptVal3dFace top( sd3dRegionShift( bot.mContour, shift ), color );

  //Side walls
  md.append( sd3dModelWalls( bot.mContour, top.mContour, color, true ) );

  //Top side
  md.append( top );

  return md;
  }
