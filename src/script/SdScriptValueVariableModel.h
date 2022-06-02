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

  Variable of 3d model
*/
#ifndef SDSCRIPTVALUEVARIABLEMODEL_H
#define SDSCRIPTVALUEVARIABLEMODEL_H

#include "SdScriptValueVariable.h"

class SdScriptValueVariableModel : public SdScriptValueVariable
  {
    Sd3drInstance mValue; //!< Model value of variable
  public:
    SdScriptValueVariableModel() : mValue() {}



    // SdScriptValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char          type() const override { return SD_SCRIPT_TYPE_MODEL; }


    //!
    //! \brief toModel Convert object to model which is compound of some bodies
    //! \return        Model which is compound of some bodies
    //!
    virtual Sd3drInstance toModel() const override { return mValue; }

    // SdScriptValueVariable interface
  public:
    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void         assign(SdScriptValuePtr src) override { mValue = src->toModel(); }
  };

#endif // SDSCRIPTVALUEVARIABLEMODEL_H
