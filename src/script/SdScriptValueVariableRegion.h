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

  Variable of region
*/
#ifndef SDSCRIPTVALUEVARIABLEREGION_H
#define SDSCRIPTVALUEVARIABLEREGION_H

#include "SdScriptValueVariable.h"

class SdScriptValueVariableRegion : public SdScriptValueVariable
  {
    SdScriptVal3dRegion mValue; //!< Region value of variable
  public:
    SdScriptValueVariableRegion() : mValue() {}



    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char                type() const override { return SD_SCRIPT_TYPE_REGION; }

    //!
    //! \brief toRegion Convert object to 3d region which is closed region of 3d vertex
    //! \return         3d region which is closed region of 3d vertex
    //!
    virtual SdScriptVal3dRegion toRegion() const override { return mValue; }

    // SdM3dVariable interface
  public:
    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void                assign(SdScriptValuePtr src) override { mValue = src->toRegion(); }
  };

#endif // SDSCRIPTVALUEVARIABLEREGION_H
