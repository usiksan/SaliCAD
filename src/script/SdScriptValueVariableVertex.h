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
    int mIndex;
  public:
    SdScriptValueVariableVertex() {}



    // SdScriptValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char type() const override { return SD_SCRIPT_TYPE_VERTEX; }

    //!
    //! \brief toVertexIndex Convert object to vertex index in Sd3dModel point list
    //! \return              Vertex index in Sd3dModel point list
    //!
    virtual int  toVertexIndex() const override { return mIndex; }

    // SdScriptValueVariable interface
  public:
    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void assign(SdScriptValuePtr src) override { mIndex = src->toVertexIndex(); }
  };

#endif // SDSCRIPTVALUEVARIABLEVERTEX_H
