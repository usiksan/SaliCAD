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
#include "SdScriptValueFunModelCopy.h"


SdScriptValueFunModelCopy::SdScriptValueFunModelCopy() : SdScriptValueFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT )
  {
  }





//!
//! \brief toModel Convert object to 3d model
//! \return        3d model
//!
SdScriptVal3dModel SdScriptValueFunModelCopy::toModel() const
  {
  QMatrix4x4 matrix;
  matrix.translate( mParamList[1]->toVertex() );
  matrix.rotate( mParamList[4]->toFloat(), QVector3D(1,0,0) );
  matrix.rotate( mParamList[3]->toFloat(), QVector3D(0,1,0) );
  matrix.rotate( mParamList[2]->toFloat(), QVector3D(0,0,1) );
  return sd3dModelMap( mParamList[0]->toModel(), matrix );
  }
