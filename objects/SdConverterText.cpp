/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdConverterText.h"

SdConverterText::SdConverterText( SdContext *context, SdPoint origin, int angle ) :
  SdConverter( context ),
  mOrigin(origin)
  {
  mAngle = static_cast<double>(angle) / 1000.0;
  }

QTransform SdConverterText::getMatrix()
  {
  //Change axiz direction
  QTransform m = QTransform::fromScale( 1.0, -1.0 );

  //At second - rotation
  m = m.rotate( mAngle );

  //At first - origin offset
  m *= QTransform::fromTranslate( mOrigin.x(), mOrigin.y() );

  return m;
  }
