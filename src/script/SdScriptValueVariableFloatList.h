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

  Variable of float list
*/
#ifndef SDSCRIPTVALUEVARIABLEFLOATLIST_H
#define SDSCRIPTVALUEVARIABLEFLOATLIST_H

#include "SdScriptValueVariable.h"

class SdScriptValueVariableFloatList : public SdScriptValueVariable
  {
    QList<float> mValue; //!< Float list value of variable
  public:
    SdScriptValueVariableFloatList() : mValue() {}

    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char  type() const override { return SD_SCRIPT_TYPE_AFLOAT; }

    //!
    //! \brief toIndex Convert object to float list
    //! \return        Float list
    //!
    virtual QList<float> toFloatList() const override { return mValue; }

    // SdM3dVariable interface
  public:
    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void assign(SdScriptValuePtr src) override { mValue = src->toFloatList(); }
  };

#endif // SDSCRIPTVALUEVARIABLEFLOATLIST_H
