/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds vertex as offset from other vertex
*/
#ifndef SDM3DFUNVERTEXOFFSET_H
#define SDM3DFUNVERTEXOFFSET_H

#include "SdM3dFunction.h"

class SdM3dFunVertexOffset : public SdM3dFunction
  {
  public:
    SdM3dFunVertexOffset() :
      SdM3dFunction( SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT )
      {

      }

    // SdM3dValue interface
  public:
    virtual QVector3D toVertex() const override
      {
      return mParamList[0]->toVertex() + QVector3D( mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat() );
      }
  };

#endif // SDM3DFUNVERTEXOFFSET_H
