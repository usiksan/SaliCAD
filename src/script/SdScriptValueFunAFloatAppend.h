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

  The function builds arc as pairs of floats (x,y)
*/
#ifndef SDSCRIPTVALUEFUNAFLOATAPPEND_H
#define SDSCRIPTVALUEFUNAFLOATAPPEND_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunAFloatAppend : public SdScriptValueFunction
  {
  public:
    //                                                                                 src                  append
    SdScriptValueFunAFloatAppend() : SdScriptValueFunction( SD_SCRIPT_TYPE_AFLOAT, SD_SCRIPT_TYPE_AFLOAT, SD_SCRIPT_TYPE_AFLOAT ) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toIndex Convert object to float list
    //! \return        Float list
    //!
    virtual QList<float>         toFloatList() const { return mParamList[0]->toFloatList() + mParamList[1]->toFloatList(); }
  };

#endif // SDSCRIPTVALUEFUNAFLOATAPPEND_H
