/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Converter for symbol and part implementation
*/
#ifndef SDCONVERTERIMPLEMENT_H
#define SDCONVERTERIMPLEMENT_H

#include "SdConverter.h"
#include "SdPoint.h"

class SdConverterImplement : public SdConverter
  {
    SdPoint mPosition; //Position of implement
    SdPoint mOrigin;   //Origin of implement
    double  mAngle;    //Angle of rotation
    bool    mMirror;   //True if implement mirrored
  public:
    SdConverterImplement( SdPoint pos, SdPoint org, int angle, bool mirror );

    // SdConverter interface
  public:
    virtual QTransform getMatrix() override;
    virtual bool       getMirror() const override;
    virtual int        getAngle() const override;
  };

#endif // SDCONVERTERIMPLEMENT_H
