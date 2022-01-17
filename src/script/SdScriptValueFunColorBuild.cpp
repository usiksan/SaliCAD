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

  Build color structure with individual color channel values
*/
#include "SdScriptValueFunColorBuild.h"


SdScriptValueFunColorBuild::SdScriptValueFunColorBuild() :
  SdScriptValueFunction( SD_SCRIPT_TYPE_COLOR, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT )
  {
  }



//!
//! \brief toColor Convert object to color
//! \return        Color
//!
QColor SdScriptValueFunColorBuild::toColor() const
  {
  return colorBuild( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat() );
  }




//!
//! \brief colorBuild Convert individual color channel values to color
//! \param r          Red
//! \param g          Green
//! \param b          Blue
//! \param a          Alpha
//! \return           Color structure
//!
QColor SdScriptValueFunColorBuild::colorBuild(float r, float g, float b, float a)
  {
  QColor color;
  color.setRgbF( r, g, b, a );
  return color;
  }
