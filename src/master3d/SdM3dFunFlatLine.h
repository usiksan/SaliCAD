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
#ifndef SDM3DFUNFLATLINE_H
#define SDM3DFUNFLATLINE_H

#include "SdM3dFunction.h"

class SdM3dFunFlatLine : public SdM3dFunction
  {
  public:
    SdM3dFunFlatLine() : SdM3dFunction( SDM3D_TYPE_FLAT, SDM3D_TYPE_VERTEX, SDM3D_TYPE_VERTEX ) {}

    // SdM3dValue interface
  public:
    virtual SdM3dFlat toFlat() const override { return SdM3dFlat( mParamList[0]->toVertex(), mParamList[1]->toVertex() ); }
  };

#endif // SDM3DFUNFLATLINE_H
