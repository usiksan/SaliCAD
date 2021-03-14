/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function finds center of region
*/
#ifndef SDM3DFUNVERTEXCENTEROFREGION_H
#define SDM3DFUNVERTEXCENTEROFREGION_H

#include "SdM3dFunction.h"

class SdM3dFunVertexCenterOfRegion : public SdM3dFunction
  {
  public:
    SdM3dFunVertexCenterOfRegion() : SdM3dFunction( SDM3D_TYPE_VERTEX, SDM3D_TYPE_REGION ) { }

    // SdM3dValue interface
  public:
    virtual QVector3D toVertex() const override { return sd3dVertexCenterOfRegion( mParamList[0]->toRegion() ); }

    static  QVector3D vertexCenterOfRegion( SdM3dRegion region );
  };

#endif // SDM3DFUNVERTEXCENTEROFREGION_H
