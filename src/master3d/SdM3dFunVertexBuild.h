/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds vertex from three coordinates
*/
#ifndef SDM3DFUNVERTEXBUILD_H
#define SDM3DFUNVERTEXBUILD_H

#include "SdM3dFunction.h"

class SdM3dFunVertexBuild : public SdM3dFunction
  {
  public:
    SdM3dFunVertexBuild() : SdM3dFunction( SDM3D_TYPE_VERTEX, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT ) { }

    // SdM3dValue interface
  public:
    virtual QVector3D toVertex() const override { return QVector3D( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat() ); }
  };

#endif // SDM3DFUNVERTEXBUILD_H
