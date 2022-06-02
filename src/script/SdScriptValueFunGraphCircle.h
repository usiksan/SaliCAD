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

  The function builds flat circle from its center vertex and radius
*/
#ifndef SDSCRIPTVALUEFUNGRAPHCIRCLE_H
#define SDSCRIPTVALUEFUNGRAPHCIRCLE_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunGraphCircle : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunGraphCircle() : SdScriptValueFunction( SD_SCRIPT_TYPE_GRAPH, SD_SCRIPT_TYPE_AFLOAT ) {}

    // SdM3dValue interface
  public:
    //!
    //! \brief toGraph Convert object to 2d graph
    //! \return        2d graph
    //!
    virtual SdScriptVal2dGraph toGraph() const override { return SdScriptVal2dGraph( mParamList[0]->toFloatList() ); }
  };

#endif // SDSCRIPTVALUEFUNGRAPHCIRCLE_H
