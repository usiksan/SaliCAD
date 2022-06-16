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

  The function builds rectangle region with rounded corners and with center in 0
*/
#ifndef SDSCRIPTVALUEFUNAFLOATARC_H
#define SDSCRIPTVALUEFUNAFLOATARC_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunAFloatArc : public SdScriptValueFunction
  {
  public:
    //                                                                                 radius                angleStart              angleStop           sideCount
    SdScriptValueFunAFloatArc() : SdScriptValueFunction( SD_SCRIPT_TYPE_AFLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toIndex Convert object to float list
    //! \return        Float list
    //!
    virtual QList<float>         toFloatList() const { return Sd3dModel::afloatArc( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat() ); }
  };

#endif // SDSCRIPTVALUEFUNAFLOATARC_H
