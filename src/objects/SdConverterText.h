/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Converter transformation for text drawing
*/
#ifndef SDCONVERTERTEXT_H
#define SDCONVERTERTEXT_H

#include "SdConverter.h"
#include "SdPoint.h"

class SdConverterText : public SdConverter
  {
    SdPoint mOrigin; //Origin point of text
    double  mAngle;  //Angle of text rotation
    bool    mMirror;
  public:
    SdConverterText( SdPoint origin, SdPvAngle angle, bool mirror );

    //Return local conversion transformation matrix
    virtual QTransform getMatrix() override;

  };

#endif // SDCONVERTERTEXT_H
