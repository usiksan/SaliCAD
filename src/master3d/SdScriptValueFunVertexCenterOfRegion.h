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

  The function finds center of region
*/
#ifndef SDSCRIPTVALUEFUNVERTEXCENTEROFREGION_H
#define SDSCRIPTVALUEFUNVERTEXCENTEROFREGION_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunVertexCenterOfRegion : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunVertexCenterOfRegion() : SdScriptValueFunction( SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_REGION ) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toVertex Convert object to 3d vertex
    //! \return         3d vertex
    //!
    virtual QVector3D toVertex() const override { return sd3dVertexCenterOfRegion( mParamList[0]->toRegion() ); }
  };

#endif // SDSCRIPTVALUEFUNVERTEXCENTEROFREGION_H
