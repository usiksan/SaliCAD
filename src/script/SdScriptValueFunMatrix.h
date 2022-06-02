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
#ifndef SDSCRIPTVALUEFUNMATRIX_H
#define SDSCRIPTVALUEFUNMATRIX_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunMatrix : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunMatrix() : SdScriptValueFunction( SD_SCRIPT_TYPE_MATRIX, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ) { }

    //!
    //! \brief toMatrix Convert object to 4x4 matrix
    //! \return         4x4 matrix
    //!
    virtual QMatrix4x4           toMatrix() const override
      {
      QMatrix4x4 map;
      map.rotate( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat() );
      map.translate( mParamList[4]->toFloat(), mParamList[5]->toFloat(), mParamList[6]->toFloat() );
      return map;
      }

  };


#endif // SDSCRIPTVALUEFUNMATRIX_H
