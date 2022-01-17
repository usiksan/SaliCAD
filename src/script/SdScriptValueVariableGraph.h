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

  Variable of 2d graph
*/
#ifndef SDSCRIPTVALUEVARIABLEGRAPH_H
#define SDSCRIPTVALUEVARIABLEGRAPH_H

#include "SdScriptValueVariable.h"

class SdScriptValueVariableGraph : public SdScriptValueVariable
  {
    SdScriptVal2dGraph mValue; //!< 2d graph value of variable
  public:
    SdScriptValueVariableGraph() : mValue(QVector3D(), QVector3D()) {}



    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char               type() const override { return SD_SCRIPT_TYPE_GRAPH; }

    //!
    //! \brief toGraph Convert object to 2d graph
    //! \return        2d graph
    //!
    virtual SdScriptVal2dGraph toGraph() const override { return mValue; }

    // SdM3dVariable interface
  public:
    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void               assign(SdScriptValuePtr src) override { mValue = src->toGraph(); }
  };

#endif // SDSCRIPTVALUEVARIABLEGRAPH_H
