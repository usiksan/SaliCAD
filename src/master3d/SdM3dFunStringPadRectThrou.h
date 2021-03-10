/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds string of pad description for rectangle pad with through hole
*/
#ifndef SDM3DFUNSTRINGPADRECTTHROU_H
#define SDM3DFUNSTRINGPADRECTTHROU_H

#include "SdM3dFunction.h"

class SdM3dFunStringPadRectThrou : public SdM3dFunction
  {
  public:
    SdM3dFunStringPadRectThrou() : SdM3dFunction( SDM3D_TYPE_STRING, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT ) { }

    // SdM3dValue interface
  public:
    virtual QString toString() const override
      {
      return QString("r%1x%2d%3m%4").arg( mParamList[0]->toFloat(), 0, 'f', 1 )
          .arg( mParamList[1]->toFloat(), 0, 'f', 1 )
          .arg( mParamList[2]->toFloat(), 0, 'f', 1 )
          .arg( mParamList[3]->toFloat(), 0, 'f', 1 );
      }
  };

#endif // SDM3DFUNSTRINGPADRECTTHROU_H
