/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPartImpProp.h"


void SdPartImpProp::operator =(const SdPartImpProp &sour)
  {
  mAngle  = sour.mAngle;
  mMirror = sour.mMirror;
  mSide   = sour.mSide;
  }

void SdPartImpProp::append(const SdPartImpProp &sour)
  {
  mAngle.append( sour.mAngle );
  mMirror.append( sour.mMirror );
  mSide.append( sour.mSide );
  }

void SdPartImpProp::clear()
  {
  mAngle.clear();
  mMirror.clear();
  mSide.clear();
  }

bool SdPartImpProp::match(const SdPartImpProp &prop)
  {
  return mAngle.match( prop.mAngle ) && mMirror.match( prop.mMirror ) && mSide.match( prop.mSide );
  }

void SdPartImpProp::write(QJsonObject &obj)
  {
  mAngle.write( QStringLiteral("PartImpAngle"), obj );
  mMirror.write( QStringLiteral("PartImpMirror"), obj );
  mSide.write( QStringLiteral("PartImpSide"), obj );
  }

void SdPartImpProp::read(const QJsonObject obj)
  {
  mAngle.read( QStringLiteral("PartImpAngle"), obj );
  mMirror.read( QStringLiteral("PartImpMirror"), obj );
  mSide.read( QStringLiteral("PartImpSide"), obj );
  }

