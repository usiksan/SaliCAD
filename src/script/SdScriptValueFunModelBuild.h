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
#ifndef SDSCRIPTVALUEFUNMODELBUILD_H
#define SDSCRIPTVALUEFUNMODELBUILD_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunModelBuild : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunModelBuild() : SdScriptValueFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_COLOR, SD_SCRIPT_TYPE_COLOR, SD_SCRIPT_TYPE_COLOR, SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_MATRIX ) { }

    //!
    //! \brief toModel Convert object to model which is compound of some bodies
    //! \return        Model which is compound of some bodies
    //!
    virtual Sd3drInstance toModel() const override {
      Sd3drBody body;
      Sd3dMaterial color( mParamList[0]->toColor(), mParamList[1]->toColor(), mParamList[2]->toColor() );
      body.colorListSet( color );
      body.faceAppend( mParamList[3]->toFaceList() );
      Sd3drInstance inst;
      inst.add( body );
      inst.addCopy( mParamList[4]->toMatrix() );
      return inst;
      }

  };

#endif // SDSCRIPTVALUEFUNMODELBUILD_H
