/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds rectangle region with center in 0
*/
#ifndef SDM3DFUNREGIONRECT_H
#define SDM3DFUNREGIONRECT_H

#include "SdM3dFunction.h"

class SdM3dFunRegionRect : public SdM3dFunction
  {
  public:
    SdM3dFunRegionRect() : SdM3dFunction( SD_SCRIPT_TYPE_REGION, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ) { }

    // SdM3dValue interface
  public:
    virtual SdScriptVal3dRegion toRegion() const override { return sd3dRegionRectangle( mParamList[0]->toFloat(), mParamList[1]->toFloat() ); }

  };

#endif // SDM3DFUNREGIONRECT_H
