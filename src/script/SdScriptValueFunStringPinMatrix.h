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

  The function builds string of pin matrix, i.e. 'A1', 'D2' and so on
*/
#ifndef SDSCRIPTVALUEFUNSTRINGPINMATRIX_H
#define SDSCRIPTVALUEFUNSTRINGPINMATRIX_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunStringPinMatrix : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunStringPinMatrix() : SdScriptValueFunction( SD_SCRIPT_TYPE_STRING, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toString Convert object to string
    //! \return         String
    //!
    virtual QString toString() const override
      {
      return QString("%1%2").arg( 'A' + static_cast<int>(mParamList[0]->toFloat()) ).arg( static_cast<int>(mParamList[0]->toFloat()) );
      }
  };

#endif // SDSCRIPTVALUEFUNSTRINGPINMATRIX_H
