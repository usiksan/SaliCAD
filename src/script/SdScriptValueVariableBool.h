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

  Variable of bool
*/
#ifndef SDSCRIPTVALUEVARIABLEBOOL_H
#define SDSCRIPTVALUEVARIABLEBOOL_H

#include "SdScriptValueVariable.h"

class SdScriptValueVariableBool : public SdScriptValueVariable
  {
    bool mValue; //!< Bool value of variable
  public:
    SdScriptValueVariableBool() : mValue(false) {}



    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char type() const override { return SD_SCRIPT_TYPE_BOOL; }

    //!
    //! \brief toBool Convert object to bool value
    //! \return       bool value
    //!
    virtual bool toBool() const override { return mValue; }

    // SdM3dVariable interface
  public:
    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void assign(SdScriptValuePtr src) override { mValue = src->toBool(); }

  };


#endif // SDSCRIPTVALUEVARIABLEBOOL_H
