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
#include "SdConverterImplement.h"


SdConverterImplement::SdConverterImplement(SdPoint pos, SdPoint org, int angle, bool mirror, bool sideOther) :
  SdConverter(),
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




bool SdConverterImplement::getMirror() const
  {
  if( mNext )
    return mMirror ? !mNext->getMirror() : mNext->getMirror();
  return mMirror;
  }




int SdConverterImplement::getAngle() const
  {
  if( mNext )
    return mAngle * 1000.0 + mNext->getAngle();
  return mAngle * 1000.0;
  }
