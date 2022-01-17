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

  Variable of path
*/
#ifndef SDSCRIPTVALUEVARIABLEPATH_H
#define SDSCRIPTVALUEVARIABLEPATH_H

#include "SdScriptValueVariable.h"

class SdScriptValueVariablePath : public SdScriptValueVariable
  {
    SdScriptVal3dPath mValue; //!< Path value of variable
  public:
    SdScriptValueVariablePath() : mValue() {}



    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char              type() const override { return SD_SCRIPT_TYPE_PATH; }

    //!
    //! \brief toPath Convert object to path of 3d segments which may be separated
    //! \return       Path of 3d segments which may be separated
    //!
    virtual SdScriptVal3dPath toPath() const override { return mValue; }

    // SdM3dVariable interface
  public:
    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void              assign(SdScriptValuePtr src) override { mValue = src->toPath(); }
  };

#endif // SDSCRIPTVALUEVARIABLEPATH_H
