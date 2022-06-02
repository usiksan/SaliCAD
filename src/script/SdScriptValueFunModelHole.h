/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Script language is C-style hi-level language for programming 3d models, 2d parametric graphics and
  schematic calculations.

  Embedded function.

  The function builds model from outer region and inner (hole) region
*/
#ifndef SDSCRIPTVALUEFUNMODELHOLE_H
#define SDSCRIPTVALUEFUNMODELHOLE_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunModelHole : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunModelHole();

    // SdM3dValue interface
  public:
    //!
    //! \brief toModel Convert object to 3d model
    //! \return        3d model
    //!
//    virtual SdScriptVal3dModel toModel() const override;

//    static  SdScriptVal3dModel modelHole( SdScriptVal3dRegion outer, SdScriptVal3dRegion hole, QColor color );

//    static  SdScriptVal3dModel modelHoleSquareCircle( SdScriptVal3dRegion square, SdScriptVal3dRegion circle, QColor color );
  };

#endif // SDSCRIPTVALUEFUNMODELHOLE_H
