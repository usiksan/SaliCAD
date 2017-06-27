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
#include "SdConverterText.h"

SdConverterText::SdConverterText(SdPoint origin, int angle , bool mirror) :
  SdConverter(),
  mOrigin(origin),
  mMirror(mirror)
  {
  mAngle = static_cast<double>(angle % 360000) / 1000.0;
  }

QTransform SdConverterText::getMatrix()
  {
  //At frist - rotation
  QTransform m = QTransform::fromScale( 1.0, 1.0 ).rotate( -mAngle );

  //At second - origin offset
  m *= QTransform::fromTranslate( mOrigin.x(), mOrigin.y() );

  return m;
  }
