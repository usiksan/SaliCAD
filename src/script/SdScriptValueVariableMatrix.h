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

  Variable of 4x4 matrix
*/
#ifndef SDSCRIPTVALUEVARIABLEMATRIX_H
#define SDSCRIPTVALUEVARIABLEMATRIX_H

#include "SdScriptValueVariable.h"

class SdScriptValueVariableMatrix : public SdScriptValueVariable
  {
    QMatrix4x4 mValue; //!< Matrix value of variable
  public:
    SdScriptValueVariableMatrix() : mValue() {}



    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char       type() const override { return SD_SCRIPT_TYPE_MATRIX; }

    //!
    //! \brief toMatrix Convert object to 4x4 matrix
    //! \return         4x4 matrix
    //!
    virtual QMatrix4x4 toMatrix() const override { return mValue; }

    // SdM3dVariable interface
  public:
    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void       assign(SdScriptValuePtr src) override { mValue = src->toMatrix(); }
  };

#endif // SDSCRIPTVALUEVARIABLEMATRIX_H
