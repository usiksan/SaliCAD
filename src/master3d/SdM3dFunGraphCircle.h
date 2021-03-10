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
    SdM3dFunGraphCircle() : SdM3dFunction( SDM3D_TYPE_GRAPH, SDM3D_TYPE_VERTEX, SDM3D_TYPE_FLOAT ) {}

    // SdM3dValue interface
  public:
    virtual SdM3dGraph toGraph() const override { return SdM3dGraph( mParamList[0]->toVertex(), mParamList[1]->toFloat() ); }
  };

#endif // SDM3DFUNGRAPHCIRCLE_H
