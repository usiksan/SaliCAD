/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
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
    SdConverterImplement( SdContext *context, SdPoint pos, SdPoint org, int angle, bool mirror, bool sideOther = false );

    // SdConverter interface
  public:
    virtual QTransform getMatrix() override;
  };

#endif // SDCONVERTERIMPLEMENT_H