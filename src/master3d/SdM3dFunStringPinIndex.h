/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds string of pin index, i.e. '1', '2' and so on
*/
#ifndef SDM3DFUNSTRINGPININDEX_H
#define SDM3DFUNSTRINGPININDEX_H

#include "SdM3dFunction.h"

class SdM3dFunStringPinIndex : public SdM3dFunction
  {
  public:
    SdM3dFunStringPinIndex() :SdM3dFunction( SD_SCRIPT_TYPE_STRING, SD_SCRIPT_TYPE_FLOAT ) { }

    // SdM3dValue interface
  public:
    virtual QString toString() const override { return QString::number( static_cast<int>(mParamList[0]->toFloat()) ); }
  };

#endif // SDM3DFUNSTRINGPININDEX_H
