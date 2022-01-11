/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  Bool value
*/
#ifndef SDM3DBOOL_H
#define SDM3DBOOL_H

#include "SdScriptValue.h"

class SdM3dBool : public SdScriptValue
  {
    bool mValue; //!< Bool value
  public:
    SdM3dBool( bool val ) : SdScriptValue(), mValue(val) {}


    // SdM3dValue interface
  public:
    virtual char type() const override { return SD_SCRIPT_TYPE_BOOL; }
    virtual bool toBool() const override { return mValue; }
  };

#endif // SDM3DBOOL_H
