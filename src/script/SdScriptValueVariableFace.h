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
    Sd3drFace mFace;
  public:
    SdScriptValueVariableFace() {}



    // SdScriptValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char      type() const override { return SD_SCRIPT_TYPE_FACE; }


    //!
    //! \brief toFace Convert object to list of vertex index which is face region
    //! \return       List of vertex index which is face region
    //!
    virtual Sd3drFace toFace() const override { return mFace; }

    // SdScriptValueVariable interface
  public:
    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void      assign(SdScriptValuePtr src) override { mFace = src->toFace(); }
  };

#endif // SDSCRIPTVALUEVARIABLEFACE_H
