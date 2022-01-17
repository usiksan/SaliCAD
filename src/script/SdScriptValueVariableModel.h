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
    SdScriptVal3dModel mValue; //!< Model value of variable
  public:
    SdScriptValueVariableModel() : mValue() {}



    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char               type() const override { return SD_SCRIPT_TYPE_MODEL; }

    //!
    //! \brief toModel Convert object to 3d model
    //! \return        3d model
    //!
    virtual SdScriptVal3dModel toModel() const override { return mValue; }

    // SdM3dVariable interface
  public:
    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void               assign(SdScriptValuePtr src) override { mValue = src->toModel(); }
  };

#endif // SDSCRIPTVALUEVARIABLEMODEL_H
