/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds string of pin matrix, i.e. 'A1', 'D2' and so on
*/
#ifndef SDM3DFUNSTRINGPINMATRIX_H
#define SDM3DFUNSTRINGPINMATRIX_H

#include "SdM3dFunction.h"

class SdM3dFunStringPinMatrix : public SdM3dFunction
  {
  public:
    SdM3dFunStringPinMatrix() : SdM3dFunction( SD_SCRIPT_TYPE_STRING, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ) { }

    // SdM3dValue interface
  public:
    virtual QString toString() const override
      {
      return QString("%1%2").arg( 'A' + static_cast<int>(mParamList[0]->toFloat()) ).arg( static_cast<int>(mParamList[0]->toFloat()) );
      }
  };

#endif // SDM3DFUNSTRINGPINMATRIX_H
