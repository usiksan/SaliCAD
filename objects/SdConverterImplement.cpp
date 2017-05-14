/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdConverterImplement.h"


SdConverterImplement::SdConverterImplement(SdContext *context, SdPoint pos, SdPoint org, int angle, bool mirror, bool sideOther) :
  SdConverter(context),
  mPosition(pos),
  mOrigin(org),
  mMirror(mirror)
  {
  mAngle = static_cast<double>(angle) / 1000.0;

  if( sideOther )
    mMirror = !mMirror;

  }



QTransform SdConverterImplement::getMatrix()
  {
  //Mirroring
  QTransform m = QTransform::fromTranslate( -mOrigin.x(), -mOrigin.y() );

  m *= QTransform::fromScale( mMirror ? -1.0 : 1.0, 1.0 );

  //At second - rotation
  m *= QTransform::fromScale( 1.0, 1.0 ).rotate( mAngle );


  //At third - origin offset
  m *= QTransform::fromTranslate( mPosition.x(), mPosition.y() );

  return m;
  }
