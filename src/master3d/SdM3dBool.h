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

#include "SdM3dValue.h"

class SdM3dBool : public SdM3dValue
  {
    bool mValue; //!< Bool value
  public:
    SdM3dBool( bool val ) : SdM3dValue(), mValue(val) {}


    // SdM3dValue interface
  public:
    virtual char type() const override { return SDM3D_TYPE_BOOL; }
    virtual bool toBool() const override { return mValue; }
  };

#endif // SDM3DBOOL_H
