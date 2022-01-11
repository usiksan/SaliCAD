/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  Float value
*/
#ifndef SDM3DFLOAT_H
#define SDM3DFLOAT_H

#include "SdScriptValue.h"

class SdM3dFloat : public SdScriptValue
  {
    float mValue; //!< Float value
  public:
    SdM3dFloat( float val ) : SdScriptValue(), mValue(val) {}



    // SdM3dValue interface
  public:
    virtual char  type() const override { return SD_SCRIPT_TYPE_FLOAT; }
    virtual float toFloat() const override { return mValue; }
  };

#endif // SDM3DFLOAT_H
