/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function translates the region with offset vector
*/
#ifndef SDM3DFUNREGIONTRANSLATE_H
#define SDM3DFUNREGIONTRANSLATE_H

#include "SdM3dFunction.h"

class SdM3dFunRegionTranslate : public SdM3dFunction
  {
  public:
    SdM3dFunRegionTranslate() : SdM3dFunction( SDM3D_TYPE_REGION, SDM3D_TYPE_REGION, SDM3D_TYPE_VERTEX ) { }

    // SdM3dValue interface
  public:
    virtual SdM3dRegion toRegion() const override { return sd3dRegionTranslate( mParamList[0]->toRegion(), mParamList[1]->toVertex() ); }

  };

#endif // SDM3DFUNREGIONTRANSLATE_H
