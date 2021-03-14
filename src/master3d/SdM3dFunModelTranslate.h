/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  Creates copy of source model translated with vector
*/
#ifndef SDM3DFUNMODELTRANSLATE_H
#define SDM3DFUNMODELTRANSLATE_H

#include "SdM3dFunction.h"

class SdM3dFunModelTranslate : public SdM3dFunction
  {
  public:
    SdM3dFunModelTranslate() : SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_MODEL, SDM3D_TYPE_VERTEX ) { }

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override { return sd3dModelTranslate( mParamList[0]->toModel(), mParamList[1]->toVertex() ); }

  };

#endif // SDM3DFUNMODELTRANSLATE_H
