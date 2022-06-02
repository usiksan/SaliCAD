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
    Sd3dModel *mModel;
  public:
    SdScriptValueFunVertexOffset( Sd3dModel *model ) :
      SdScriptValueFunction( SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ),
      mModel(model)
      {

      }

    //!
    //! \brief toVertexIndex Convert object to vertex index in Sd3dModel point list
    //! \return              Vertex index in Sd3dModel point list
    //!
    virtual int toVertexIndex() const override
      {
      return mModel->vertexAppend( mModel->vertex( mParamList[0]->toVertexIndex() ) + QVector3D( mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat() )  );
      }
  };

#endif // SDSCRIPTVALUEFUNVERTEXOFFSET_H
