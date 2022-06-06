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
#ifndef SDSCRIPTVALUEFUNMATRIXROTATE_H
#define SDSCRIPTVALUEFUNMATRIXROTATE_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunMatrixRotate : public SdScriptValueFunction
  {
  public:
    //                                                                              QMatrix src                 float angle         float vx              float vy               float vz
    SdScriptValueFunMatrixRotate() : SdScriptValueFunction( SD_SCRIPT_TYPE_MATRIX, SD_SCRIPT_TYPE_MATRIX, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ) { }

    //!
    //! \brief toMatrix Convert object to 4x4 matrix
    //! \return         4x4 matrix
    //!
    virtual QMatrix4x4           toMatrix() const override
      {
      QMatrix4x4 map = mParamList[0]->toMatrix();
      map.rotate( mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat(), mParamList[4]->toFloat() );
      return map;
      }

  };


#endif // SDSCRIPTVALUEFUNMATRIXROTATE_H
