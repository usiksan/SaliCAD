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

  The function builds vertex as offset from other vertex
*/
#ifndef SDSCRIPTVALUEFUNVERTEXOFFSET_H
#define SDSCRIPTVALUEFUNVERTEXOFFSET_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunVertexOffset : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunVertexOffset() :
      SdScriptValueFunction( SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT )
      {

      }

    // SdM3dValue interface
  public:
    //!
    //! \brief toVertex Convert object to 3d vertex
    //! \return         3d vertex
    //!
    virtual QVector3D toVertex() const override
      {
      return mParamList[0]->toVertex() + QVector3D( mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat() );
      }
  };

#endif // SDSCRIPTVALUEFUNVERTEXOFFSET_H
