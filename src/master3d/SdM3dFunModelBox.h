/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds box model from its size
*/
#ifndef SDM3DFUNMODELBOX_H
#define SDM3DFUNMODELBOX_H

#include "SdM3dFunction.h"

class SdM3dFunModelBox : public SdM3dFunction
  {
  public:
    SdM3dFunModelBox() :
      SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_COLOR )
      {

      }

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override
      {
      return sd3dModelBox( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toColor() );
      }
  };

#endif // SDM3DFUNMODELBOX_H
