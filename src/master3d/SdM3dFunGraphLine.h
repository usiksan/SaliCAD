/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds flat line from its vertex
*/
#ifndef SDM3DFUNGRAPHLINE_H
#define SDM3DFUNGRAPHLINE_H

#include "SdM3dFunction.h"

class SdM3dFunGraphLine : public SdM3dFunction
  {
  public:
    SdM3dFunGraphLine() : SdM3dFunction( SD_SCRIPT_TYPE_GRAPH, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_VERTEX ) {}

    // SdM3dValue interface
  public:
    virtual SdScriptVal2dGraph toGraph() const override { return SdScriptVal2dGraph( mParamList[0]->toVertex(), mParamList[1]->toVertex() ); }
  };

#endif // SDM3DFUNGRAPHLINE_H
