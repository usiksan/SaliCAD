/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds cylinder model from its size
*/
#ifndef SDM3DFUNMODELCYLINDER_H
#define SDM3DFUNMODELCYLINDER_H

#include "SdM3dFunction.h"

class SdM3dFunModelCylinder : public SdM3dFunction
  {
  public:
    SdM3dFunModelCylinder() :
      SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_COLOR )
      {

      }

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override
      {
      return sd3dModelCylinder( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toColor() );
      }

  };

#endif // SDM3DFUNMODELCYLINDER_H
