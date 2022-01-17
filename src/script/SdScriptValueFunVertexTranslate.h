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

class SdScriptValueFunVertexTranslate : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunVertexTranslate() : SdScriptValueFunction( SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_VERTEX ) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toVertex Convert object to 3d vertex
    //! \return         3d vertex
    //!
    virtual QVector3D toVertex() const override  { return mParamList[0]->toVertex() + mParamList[1]->toVertex(); }
  };

#endif // SDSCRIPTVALUEFUNVERTEXTRANSLATE_H
