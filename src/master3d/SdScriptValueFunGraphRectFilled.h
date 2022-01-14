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

  The function builds flat filled rect from its vertex
*/
#ifndef SDSCRIPTVALUEFUNGRAPHRECTFILLED_H
#define SDSCRIPTVALUEFUNGRAPHRECTFILLED_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunGraphRectFilled : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunGraphRectFilled() : SdScriptValueFunction( SD_SCRIPT_TYPE_GRAPH, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_VERTEX ) {}

    // SdM3dValue interface
  public:
    //!
    //! \brief toGraph Convert object to 2d graph
    //! \return        2d graph
    //!
    virtual SdScriptVal2dGraph toGraph() const override { return SdScriptVal2dGraph( mParamList[0]->toVertex(), mParamList[1]->toVertex(), true ); }
  };

#endif // SDSCRIPTVALUEFUNGRAPHRECTFILLED_H
