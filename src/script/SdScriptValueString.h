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

  Represents string value
*/
#ifndef SDSCRIPTVALUESTRING_H
#define SDSCRIPTVALUESTRING_H

#include "SdScriptValue.h"

class SdScriptValueString : public SdScriptValue
  {
    QString mValue; //!< String value
  public:
    SdScriptValueString( const QString &val ) : SdScriptValue(), mValue(val) {}

    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char    type() const override { return SD_SCRIPT_TYPE_STRING;  }

    //!
    //! \brief toString Convert object to string
    //! \return         String
    //!
    virtual QString toString() const override { return mValue; }
  };

#endif // SDSCRIPTVALUESTRING_H
