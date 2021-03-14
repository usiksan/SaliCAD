/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds curved model from its bottom region and rotation params
*/
#ifndef SDM3DFUNMODELCURVE_H
#define SDM3DFUNMODELCURVE_H

#include "SdM3dFunction.h"

class SdM3dFunModelCurve : public SdM3dFunction
  {
  public:
    SdM3dFunModelCurve() :
      SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_REGION, SDM3D_TYPE_VERTEX, SDM3D_TYPE_VERTEX, SDM3D_TYPE_FLOAT, SDM3D_TYPE_COLOR, SDM3D_TYPE_BOOL )
      {

      }

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override
      {
      return sd3dModelCurve( mParamList[0]->toRegion(), mParamList[1]->toVertex(), mParamList[2]->toVertex(), mParamList[3]->toFloat(), mParamList[4]->toColor(), mParamList[5]->toBool() );
      }

  };

#endif // SDM3DFUNMODELCURVE_H
