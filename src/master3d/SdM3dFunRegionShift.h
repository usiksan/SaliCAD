/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function moves the region in the direction perpendicular to the plane of the region by the specified amount
*/
#ifndef SDM3DFUNREGIONSHIFT_H
#define SDM3DFUNREGIONSHIFT_H

#include "SdM3dFunction.h"

class SdM3dFunRegionShift : public SdM3dFunction
  {
  public:
    SdM3dFunRegionShift() : SdM3dFunction( SD_SCRIPT_TYPE_REGION, SD_SCRIPT_TYPE_REGION, SD_SCRIPT_TYPE_FLOAT ) { }

    // SdM3dValue interface
  public:
    virtual SdScriptVal3dRegion toRegion() const override { return sd3dRegionShift( mParamList[0]->toRegion(), mParamList[1]->toFloat() ); }

  };

#endif // SDM3DFUNREGIONSHIFT_H
