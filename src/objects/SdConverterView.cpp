/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Convertor for view point
*/
#include "SdConverterView.h"
#include "SdContext.h"

SdConverterView::SdConverterView(QSize viewSize, SdPoint origin, double scale, bool mirror) :
  SdConverter(),
  mCenter( viewSize.width() / 2, viewSize.height() / 2 ),
  mScale(scale),
  mOrigin(origin),
  mMirror(mirror)
  {
  }


//Return local conversion transformation matrix
QTransform SdConverterView::getMatrix()
  {
  //At first - origin offset
  QTransform m = QTransform::fromTranslate( -mOrigin.x(), -mOrigin.y() );

  //Second - scaling
  m *= QTransform::fromScale( mMirror ? -mScale : mScale, -mScale );

  //Third - center view offset
  m *= QTransform::fromTranslate( mCenter.x(), mCenter.y() );
  return m;
  }

