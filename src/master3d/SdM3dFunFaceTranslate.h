/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function translates face with offset vector
*/
#ifndef SDM3DFUNFACETRANSLATE_H
#define SDM3DFUNFACETRANSLATE_H

#include "SdM3dFunction.h"

class SdM3dFunFaceTranslate : public SdM3dFunction
  {
  public:
    SdM3dFunFaceTranslate() : SdM3dFunction( SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_VERTEX ) { }

    // SdM3dValue interface
  public:
    virtual SdScriptVal3dFace toFace() const override { return mParamList[0]->toFace().translate( mParamList[1]->toVertex() ); }
  };

#endif // SDM3DFUNFACETRANSLATE_H
