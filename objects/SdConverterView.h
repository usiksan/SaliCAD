/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDCONVERTERVIEW_H
#define SDCONVERTERVIEW_H

#include "SdConverter.h"
#include "SdPoint.h"

class SdConverterView : public SdConverter
  {
    SdPoint mCenter; //View center in pixels
    double  mScale;  //View scale
    SdPoint mOrigin; //View offset
  public:
    SdConverterView( SdContext *context, QSize viewSize, SdPoint origin, double scale );

    //Return local conversion transformation matrix
    virtual QTransform getMatrix() override;

    //Return scale factor
    virtual double     getScale() const override { return mScale; }
  };

#endif // SDCONVERTERVIEW_H
