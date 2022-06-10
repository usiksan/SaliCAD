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

  The function used to dialog select color and to convert textual color to binary color
*/
#ifndef SDSCRIPTVALUEFUNSELECTCOLOR_H
#define SDSCRIPTVALUEFUNSELECTCOLOR_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunSelectColor : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunSelectColor() : SdScriptValueFunction( SD_SCRIPT_TYPE_COLOR, SD_SCRIPT_TYPE_STRING ) {}

    // SdM3dValue interface
  public:
    //!
    //! \brief toColor Convert object to color
    //! \return        Color
    //!
    virtual QColor toColor() const override { return QColor( mParamList[0]->toString() ); }
  };


#endif // SDSCRIPTVALUEFUNSELECTCOLOR_H
