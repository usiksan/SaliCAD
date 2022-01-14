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

  Float value
*/
#ifndef SDSCRIPTVALUEFLOAT_H
#define SDSCRIPTVALUEFLOAT_H

#include "SdScriptValue.h"

class SdScriptValueFloat : public SdScriptValue
  {
    float mValue; //!< Float value
  public:
    SdScriptValueFloat( float val ) : SdScriptValue(), mValue(val) {}



    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char  type() const override { return SD_SCRIPT_TYPE_FLOAT; }

    //!
    //! \brief toFloat Convert object ot float value
    //! \return        float value
    //!
    virtual float toFloat() const override { return mValue; }
  };

#endif // SDSCRIPTVALUEFLOAT_H
