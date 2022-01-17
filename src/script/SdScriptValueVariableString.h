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

  Variable of string
*/
#ifndef SDSCRIPTVALUEVARIABLESTRING_H
#define SDSCRIPTVALUEVARIABLESTRING_H

#include "SdScriptValueVariable.h"

class SdScriptValueVariableString : public SdScriptValueVariable
  {
    QString mValue; //!< String value of variable
  public:
    SdScriptValueVariableString() {}

    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char    type() const override { return SD_SCRIPT_TYPE_STRING; }

    //!
    //! \brief toString Convert object to string
    //! \return         String
    //!
    virtual QString toString() const override { return mValue; }

    // SdM3dVariable interface
  public:
    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void    assign(SdScriptValuePtr src) override { mValue = src->toString(); }
  };

#endif // SDSCRIPTVALUEVARIABLESTRING_H
