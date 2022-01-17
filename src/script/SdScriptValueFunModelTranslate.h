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
#ifndef SDSCRIPTVALUEFUNMODELTRANSLATE_H
#define SDSCRIPTVALUEFUNMODELTRANSLATE_H

#include "SdScriptValueFunction.h"

class SdM3dFunModelTranslate : public SdScriptValueFunction
  {
  public:
    SdM3dFunModelTranslate() : SdScriptValueFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_VERTEX ) { }

    // SdM3dValue interface
  public:
    virtual SdScriptVal3dModel toModel() const override { return sd3dModelTranslate( mParamList[0]->toModel(), mParamList[1]->toVertex() ); }

  };

#endif // SDSCRIPTVALUEFUNMODELTRANSLATE_H
