/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds flat filled rect from its vertex
*/
#ifndef SDM3DFUNGRAPHRECTFILLED_H
#define SDM3DFUNGRAPHRECTFILLED_H

#include "SdM3dFunction.h"

class SdM3dFunGraphRectFilled : public SdM3dFunction
  {
  public:
    SdM3dFunGraphRectFilled() : SdM3dFunction( SDM3D_TYPE_GRAPH, SDM3D_TYPE_VERTEX, SDM3D_TYPE_VERTEX ) {}

    // SdM3dValue interface
  public:
    virtual SdM3dGraph toGraph() const override { return SdM3dGraph( mParamList[0]->toVertex(), mParamList[1]->toVertex(), true ); }
  };

#endif // SDM3DFUNGRAPHRECTFILLED_H
