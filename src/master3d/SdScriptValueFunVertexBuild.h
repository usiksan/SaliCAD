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

  The function builds vertex from three coordinates
*/
#ifndef SDSCRIPTVALUEFUNVERTEXBUILD_H
#define SDSCRIPTVALUEFUNVERTEXBUILD_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunVertexBuild : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunVertexBuild() : SdScriptValueFunction( SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toVertex Convert object to 3d vertex
    //! \return         3d vertex
    //!
    virtual QVector3D toVertex() const override { return QVector3D( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat() ); }
  };

#endif // SDSCRIPTVALUEFUNVERTEXBUILD_H
