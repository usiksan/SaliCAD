/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds face from face contour and face color
*/
#ifndef SDM3DFUNFACEBUILD_H
#define SDM3DFUNFACEBUILD_H

#include "SdM3dFunction.h"

class SdM3dFunFaceBuild : public SdM3dFunction
  {
  public:
    SdM3dFunFaceBuild() : SdM3dFunction( SDM3D_TYPE_FACE, SDM3D_TYPE_REGION, SDM3D_TYPE_COLOR ) { }

    // SdM3dValue interface
  public:
    virtual SdM3dFace toFace() const override { return Sd3dFace( mParamList[0]->toRegion(), mParamList[1]->toColor() ); }

  };


#endif // SDM3DFUNFACEBUILD_H
