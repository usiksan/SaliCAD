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

  Variable of face
*/
#ifndef SDSCRIPTVALUEVARIABLEFACE_H
#define SDSCRIPTVALUEVARIABLEFACE_H

#include "SdScriptValueVariable.h"

class SdScriptValueVariableFace : public SdScriptValueVariable
  {
    SdScriptVal3dFace mValue; //!< Face value of variable
  public:
    SdScriptValueVariableFace() : mValue() {}



    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char              type() const override { return SD_SCRIPT_TYPE_FACE; }

    //!
    //! \brief toFace Convert object to 3d face which is bound region and face color
    //! \return       3d face which is bound region and face color
    //!
    virtual SdScriptVal3dFace toFace() const override { return mValue; }

    // SdM3dVariable interface
  public:
    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void              assign(SdScriptValuePtr src) override { mValue = src->toFace(); }
  };

#endif // SDSCRIPTVALUEVARIABLEFACE_H
