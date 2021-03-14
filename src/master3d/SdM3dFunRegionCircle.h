/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function build circle region
*/
#ifndef SDM3DFUNREGIONCIRCLE_H
#define SDM3DFUNREGIONCIRCLE_H

#include "SdM3dFunction.h"

class SdM3dFunRegionCircle : public SdM3dFunction
  {
  public:
    SdM3dFunRegionCircle() : SdM3dFunction( SDM3D_TYPE_REGION, SDM3D_TYPE_FLOAT ) { }

    // SdM3dValue interface
  public:
    virtual SdM3dRegion toRegion() const override { return sd3dRegionCircle( mParamList[0]->toFloat() ); }

  };

#endif // SDM3DFUNREGIONCIRCLE_H
