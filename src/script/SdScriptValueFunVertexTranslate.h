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

  The function makes summ of two vectors
*/
#ifndef SDSCRIPTVALUEFUNVERTEXTRANSLATE_H
#define SDSCRIPTVALUEFUNVERTEXTRANSLATE_H

#include "SdScriptValueFunction.h"
#include "objects/Sd3dModel.h"

class SdScriptValueFunVertexTranslate : public SdScriptValueFunction
  {
    Sd3dModel *mModel;
  public:
    SdScriptValueFunVertexTranslate( Sd3dModel *model ) :
      SdScriptValueFunction( SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_VERTEX ),
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
      QVector3D v = mModel->vertex( mParamList[0]->toIndex() ) + mModel->vertex( mParamList[1]->toIndex() );
      return QList<int>( {mModel->vertexAppend(v)} );
      }
  };

#endif // SDSCRIPTVALUEFUNVERTEXTRANSLATE_H
