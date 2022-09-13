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

  The function builds face from face contour and face color
*/
#ifndef SDSCRIPTVALUEFUNFACEPART_H
#define SDSCRIPTVALUEFUNFACEPART_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunFacePart : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunFacePart() : SdScriptValueFunction( SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_AFLOAT ) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toFace Convert object to list of vertex index which is face region
    //! \return       List of vertex index which is face region
    //!
    virtual Sd3drFace toFace() const override { return Sd3drModel::facePart( mParamList[0]->toFace(), mParamList[1]->toFloatList() ); }

  };


#endif // SDSCRIPTVALUEFUNFACEPART_H
