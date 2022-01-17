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

  Variable of color
*/
#ifndef SDSCRIPTVALUEVARIABLECOLOR_H
#define SDSCRIPTVALUEVARIABLECOLOR_H

#include "SdScriptValueVariable.h"

class SdScriptValueVariableColor : public SdScriptValueVariable
  {
    QColor mValue; //!< Color value of variable
  public:
    SdScriptValueVariableColor() : mValue("black") {}



    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char   type() const override { return SD_SCRIPT_TYPE_COLOR; }

    //!
    //! \brief toColor Convert object to color
    //! \return        Color
    //!
    virtual QColor toColor() const override { return mValue; }

    // SdM3dVariable interface
  public:
    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void assign(SdScriptValuePtr src) override { mValue = src->toColor(); }
  };

#endif // SDSCRIPTVALUEVARIABLECOLOR_H
