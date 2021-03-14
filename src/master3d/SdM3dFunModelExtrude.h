/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function extrudes the model from the region in the direction of the vector
*/

#ifndef SDM3DFUNMODELEXTRUDE_H
#define SDM3DFUNMODELEXTRUDE_H

#include "SdM3dFunction.h"

class SdM3dFunModelExtrude : public SdM3dFunction
  {
  public:
    SdM3dFunModelExtrude() :
      SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_REGION, SDM3D_TYPE_FLOAT, SDM3D_TYPE_COLOR )
      {

      }

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override
      {
      return sd3dModelExtrude( mParamList[0]->toRegion(), mParamList[1]->toFloat(), mParamList[2]->toColor() );
      }

  };

#endif // SDM3DFUNMODELEXTRUDE_H
