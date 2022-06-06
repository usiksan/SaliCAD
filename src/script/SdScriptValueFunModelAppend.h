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
#ifndef SDSCRIPTVALUEFUNMODELAPPEND_H
#define SDSCRIPTVALUEFUNMODELAPPEND_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunModelAppend : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunModelAppend() : SdScriptValueFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_COLOR, SD_SCRIPT_TYPE_COLOR, SD_SCRIPT_TYPE_COLOR, SD_SCRIPT_TYPE_FACE_LIST ) { }

    //!
    //! \brief toModel Convert object to model which is compound of some bodies
    //! \return        Model which is compound of some bodies
    //!
    virtual Sd3drInstance toModel() const override {
      Sd3drBody body;
      Sd3dMaterial color( mParamList[1]->toColor(), mParamList[2]->toColor(), mParamList[3]->toColor() );
      body.colorListSet( color );
      body.faceAppend( mParamList[4]->toFaceList() );
      Sd3drInstance inst = mParamList[0]->toModel();
      inst.add( body );
      return inst;
      }

  };


#endif // SDSCRIPTVALUEFUNMODELAPPEND_H
