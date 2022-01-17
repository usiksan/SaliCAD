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

  The function builds triangle face with three vertexies and color
*/
#ifndef SDSCRIPTVALUEFUNFACETRIANGLE_H
#define SDSCRIPTVALUEFUNFACETRIANGLE_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunFaceTriangle : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunFaceTriangle() : SdScriptValueFunction( SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_COLOR ) { }


    // SdM3dValue interface
  public:
    //!
    //! \brief toFace Convert object to 3d face which is bound region and face color
    //! \return       3d face which is bound region and face color
    //!
    virtual SdScriptVal3dFace toFace() const override { return Sd3dFace( mParamList[0]->toVertex(), mParamList[1]->toVertex(), mParamList[2]->toVertex(), mParamList[3]->toColor() ); }

  };

#endif // SDSCRIPTVALUEFUNFACETRIANGLE_H
