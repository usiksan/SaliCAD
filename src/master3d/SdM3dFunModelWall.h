/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds wall from bottom region and grow vector
*/
#ifndef SDM3DFUNMODELWALL_H
#define SDM3DFUNMODELWALL_H

#include "SdM3dFunction.h"

class SdM3dFunModelWall : public SdM3dFunction
  {
  public:
    SdM3dFunModelWall() :
      SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_REGION, SDM3D_TYPE_VERTEX, SDM3D_TYPE_COLOR, SDM3D_TYPE_BOOL )
      {

      }

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override
      {
      return sd3dModelWall( mParamList[0]->toRegion(), mParamList[1]->toVertex(), mParamList[2]->toColor(), mParamList[3]->toBool() );
      }

  };

#endif // SDM3DFUNMODELWALL_H
