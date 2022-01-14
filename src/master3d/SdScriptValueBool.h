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

  Bool value
*/
#ifndef SDSCRIPTVALUEBOOL_H
#define SDSCRIPTVALUEBOOL_H

#include "SdScriptValue.h"

class SdScriptValueBool : public SdScriptValue
  {
    bool mValue; //!< Bool value
  public:
    SdScriptValueBool( bool val ) : SdScriptValue(), mValue(val) {}


    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char type() const override { return SD_SCRIPT_TYPE_BOOL; }

    //!
    //! \brief toBool Convert object to bool value
    //! \return       bool value
    //!
    virtual bool toBool() const override { return mValue; }
  };

#endif // SDSCRIPTVALUEBOOL_H
