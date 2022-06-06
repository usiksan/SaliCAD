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

  Variable of float
*/
#ifndef SDSCRIPTVALUEVARIABLEFLOAT_H
#define SDSCRIPTVALUEVARIABLEFLOAT_H

#include "SdScriptValueVariable.h"

class SdScriptValueVariableFloat : public SdScriptValueVariable
  {
    float mValue; //!< Float value of variable
  public:
    SdScriptValueVariableFloat() : mValue(0.0) {}
    SdScriptValueVariableFloat( float f ) : mValue(f) {}

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

    // SdM3dVariable interface
  public:
    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void assign(SdScriptValuePtr src) override { mValue = src->toFloat(); }
  };

#endif // SDSCRIPTVALUEVARIABLEFLOAT_H
