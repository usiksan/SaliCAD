/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdConverterView.h"
#include "SdContext.h"

SdConverterView::SdConverterView(SdContext *context, QSize viewSize, SdPoint origin, double scale) :
  SdConverter (context),
  mCenter( viewSize.width() / 2, viewSize.height() / 2 ),
  mScale(scale),
  mOrigin(origin)
  {
  context->setConverter( this );
  }


//Return local conversion transformation matrix
QTransform SdConverterView::getMatrix()
  {
  //At first - origin offset
  QTransform m = QTransform::fromTranslate( -mOrigin.x(), -mOrigin.y() );

  //Second - scaling
  m *= QTransform::fromScale( mScale, -mScale );

  //Third - center view offset
  m *= QTransform::fromTranslate( mCenter.x(), mCenter.y() );
  return m;
  }

