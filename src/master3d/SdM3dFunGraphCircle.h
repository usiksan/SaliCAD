/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds flat circle from its center vertex and radius
*/
#ifndef SDM3DFUNGRAPHCIRCLE_H
#define SDM3DFUNGRAPHCIRCLE_H

#include "SdM3dFunction.h"

class SdM3dFunGraphCircle : public SdM3dFunction
  {
  public:
    SdM3dFunGraphCircle() : SdM3dFunction( SD_SCRIPT_TYPE_GRAPH, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_FLOAT ) {}

    // SdM3dValue interface
  public:
    virtual SdScriptVal2dGraph toGraph() const override { return SdScriptVal2dGraph( mParamList[0]->toVertex(), mParamList[1]->toFloat() ); }
  };

#endif // SDM3DFUNGRAPHCIRCLE_H
