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

  Variable of vertex
*/
#ifndef SDSCRIPTVALUEVARIABLEVERTEX_H
#define SDSCRIPTVALUEVARIABLEVERTEX_H

#include "SdScriptValueVariable.h"

class SdScriptValueVariableVertex : public SdScriptValueVariable
  {
    QVector3D mValue; //!< Vertex value of variable
  public:
    SdScriptValueVariableVertex() : mValue() {}



    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char      type() const override { return SD_SCRIPT_TYPE_VERTEX; }

    //!
    //! \brief toVertex Convert object to 3d vertex
    //! \return         3d vertex
    //!
    virtual QVector3D toVertex() const override { return mValue; }

    // SdM3dVariable interface
  public:
    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void      assign(SdScriptValuePtr src) override { mValue = src->toVertex(); }
  };

#endif // SDSCRIPTVALUEVARIABLEVERTEX_H
