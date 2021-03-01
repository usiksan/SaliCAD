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
    virtual SdM3dModel toModel() const override;

    static  SdM3dModel modelHole( SdM3dRegion outer, SdM3dRegion hole, QColor color );

    static  SdM3dModel modelHoleSquareCircle( SdM3dRegion square, SdM3dRegion circle, QColor color );
  };

#endif // SDM3DFUNMODELHOLE_H
