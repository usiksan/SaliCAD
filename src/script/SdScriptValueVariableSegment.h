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

  Variable of segment
*/
#ifndef SDSCRIPTVALUEVARIABLESEGMENT_H
#define SDSCRIPTVALUEVARIABLESEGMENT_H

#include "SdScriptValueVariable.h"

class SdScriptValueVariableSegment : public SdScriptValueVariable
  {
    SdScriptVal3dSegment mValue; //!< Segment value of variable
  public:
    SdScriptValueVariableSegment() : mValue() {}



    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char                 type() const override { return SD_SCRIPT_TYPE_SEGMENT; }

    //!
    //! \brief toSegment Convert object to 3d segment
    //! \return          3d segment
    //!
    virtual SdScriptVal3dSegment toSegment() const override { return mValue; }

    // SdM3dVariable interface
  public:
    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void                 assign(SdScriptValuePtr src) override { mValue = src->toSegment(); }
  };

#endif // SDSCRIPTVALUEVARIABLESEGMENT_H
