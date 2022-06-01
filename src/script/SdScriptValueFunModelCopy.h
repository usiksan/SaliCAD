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

  The function copy model to other position and orientation
*/
#ifndef SDSCRIPTVALUEFUNMODELCOPY_H
#define SDSCRIPTVALUEFUNMODELCOPY_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunModelCopy : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunModelCopy() : SdScriptValueFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_MATRIX )
      {
      }

    //!
    //! \brief toModel Convert object to model which is compound of some bodies
    //! \return        Model which is compound of some bodies
    //!
    virtual Sd3drInstance        toModel() const override
      {
      Sd3drInstance model = mParamList[0]->toModel();
      model.addCopy( mParamList[1]->toMatrix() );
      return model;
      }

  };

#endif // SDSCRIPTVALUEFUNMODELCOPY_H
