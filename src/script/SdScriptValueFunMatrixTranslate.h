/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  Creates translation and rotation matrix
*/
#ifndef SDSCRIPTVALUEFUNMATRIXTRANSLATE_H
#define SDSCRIPTVALUEFUNMATRIXTRANSLATE_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunMatrixTranslate : public SdScriptValueFunction
  {
  public:
    //                                                                                 QMatrix src                 float vx              float vy               float vz
    SdScriptValueFunMatrixTranslate() : SdScriptValueFunction( SD_SCRIPT_TYPE_MATRIX, SD_SCRIPT_TYPE_MATRIX, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ) { }

    //!
    //! \brief toMatrix Convert object to 4x4 matrix
    //! \return         4x4 matrix
    //!
    virtual QMatrix4x4           toMatrix() const override
      {
      QMatrix4x4 map = mParamList[0]->toMatrix();
      map.translate( mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat() );
      return map;
      }

  };


#endif // SDSCRIPTVALUEFUNMATRIXTRANSLATE_H
