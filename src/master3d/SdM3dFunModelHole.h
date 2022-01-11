/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds model from outer region and inner (hole) region
*/
#ifndef SDM3DFUNMODELHOLE_H
#define SDM3DFUNMODELHOLE_H

#include "SdM3dFunction.h"

class SdM3dFunModelHole : public SdM3dFunction
  {
  public:
    SdM3dFunModelHole();

    // SdM3dValue interface
  public:
    virtual SdScriptVal3dModel toModel() const override;

    static  SdScriptVal3dModel modelHole( SdScriptVal3dRegion outer, SdScriptVal3dRegion hole, QColor color );

    static  SdScriptVal3dModel modelHoleSquareCircle( SdScriptVal3dRegion square, SdScriptVal3dRegion circle, QColor color );
  };

#endif // SDM3DFUNMODELHOLE_H
