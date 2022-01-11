/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds triangle face with three vertexies and color
*/
#ifndef SDM3DFUNFACETRIANGLE_H
#define SDM3DFUNFACETRIANGLE_H

#include "SdM3dFunction.h"

class SdM3dFunFaceTriangle : public SdM3dFunction
  {
  public:
    SdM3dFunFaceTriangle() : SdM3dFunction( SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_COLOR ) { }


    // SdM3dValue interface
  public:
    virtual SdScriptVal3dFace toFace() const override { return Sd3dFace( mParamList[0]->toVertex(), mParamList[1]->toVertex(), mParamList[2]->toVertex(), mParamList[3]->toColor() ); }

  };

#endif // SDM3DFUNFACETRIANGLE_H
