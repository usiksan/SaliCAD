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

  The function builds string of pin index, i.e. '1', '2' and so on
*/
#ifndef SDSCRIPTVALUEFUNSTRINGPININDEX_H
#define SDSCRIPTVALUEFUNSTRINGPININDEX_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunStringPinIndex : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunStringPinIndex() :SdScriptValueFunction( SD_SCRIPT_TYPE_STRING, SD_SCRIPT_TYPE_FLOAT ) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toString Convert object to string
    //! \return         String
    //!
    virtual QString toString() const override { return QString::number( static_cast<int>(mParamList[0]->toFloat()) ); }
  };

#endif // SDSCRIPTVALUEFUNSTRINGPININDEX_H
