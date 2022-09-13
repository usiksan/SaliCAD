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
#include "3dr/Sd3drModel.h"

class SdScriptValueFunVertexBuild : public SdScriptValueFunction
  {
    Sd3drModel *mModel;
  public:
    SdScriptValueFunVertexBuild( Sd3drModel *model ) :
      SdScriptValueFunction( SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ),
      mModel(model)
      { }

    // SdScriptValue interface
  public:
    //!
    //! \brief toVertexIndex Convert object to vertex index in Sd3dModel point list
    //! \return              Vertex index in Sd3dModel point list
    //!
    virtual int toVertexIndex() const override
      {
      QVector3D v( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat() );
      return mModel->vertexAppend( v );
      }
  };

#endif // SDSCRIPTVALUEFUNVERTEXBUILD_H
