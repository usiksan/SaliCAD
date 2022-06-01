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
#include "objects/Sd3dModel.h"

class SdScriptValueFunVertexBuild : public SdScriptValueFunction
  {
    Sd3dModel *mModel;
  public:
    SdScriptValueFunVertexBuild( Sd3dModel *model ) :
      SdScriptValueFunction( SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ),
      mModel(model)
      { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toIndex Convert object to index list in reference list
    //! \return        Index list in reference list
    //!
    virtual QList<int>           toIndexList() const override
      {
      QVector3D v( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat() );
      return QList<int>( {mModel->vertexAppend(v)} );
      }

  };

#endif // SDSCRIPTVALUEFUNVERTEXBUILD_H
