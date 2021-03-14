/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function makes summ of two vectors
*/
#ifndef SDM3DFUNVERTEXTRANSLATE_H
#define SDM3DFUNVERTEXTRANSLATE_H

#include "SdM3dFunction.h"

class SdM3dFunVertexTranslate : public SdM3dFunction
  {
  public:
    SdM3dFunVertexTranslate() : SdM3dFunction( SDM3D_TYPE_VERTEX, SDM3D_TYPE_VERTEX, SDM3D_TYPE_VERTEX ) { }

    // SdM3dValue interface
  public:
    virtual QVector3D toVertex() const override  { return mParamList[0]->toVertex() + mParamList[1]->toVertex(); }
  };

#endif // SDM3DFUNVERTEXTRANSLATE_H
