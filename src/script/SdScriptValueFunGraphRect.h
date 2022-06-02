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

  The function builds flat rect from its vertex
*/
#ifndef SDSCRIPTVALUEFUNGRAPHRECT_H
#define SDSCRIPTVALUEFUNGRAPHRECT_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunGraphRect : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunGraphRect() : SdScriptValueFunction( SD_SCRIPT_TYPE_GRAPH, SD_SCRIPT_TYPE_AFLOAT, SD_SCRIPT_TYPE_AFLOAT ) {}

    // SdM3dValue interface
  public:
    //!
    //! \brief toGraph Convert object to 2d graph
    //! \return        2d graph
    //!
    virtual SdScriptVal2dGraph toGraph() const override { return SdScriptVal2dGraph( mParamList[0]->toFloatList(), mParamList[1]->toFloatList(), false ); }
  };

#endif // SDSCRIPTVALUEFUNGRAPHRECT_H
