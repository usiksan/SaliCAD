/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdSymImpProp.h"


void SdSymImpProp::operator =(const SdSymImpProp &sour)
  {
  mAngle  = sour.mAngle;
  mMirror = sour.mMirror;
  }

void SdSymImpProp::append(const SdSymImpProp &sour)
  {
  mAngle.append( sour.mAngle );
  mMirror.append( sour.mMirror );
  }

void SdSymImpProp::clear()
  {
  mAngle.clear();
  mMirror.clear();
  }

bool SdSymImpProp::match(const SdSymImpProp &prop)
  {
  return mAngle.match( prop.mAngle ) && mMirror.match( prop.mMirror );
  }

void SdSymImpProp::write(QJsonObject &obj) const
  {
  mAngle.write( QStringLiteral("SymImpAngle"), obj );
  mMirror.write( QStringLiteral("SymImpMirror"), obj );
  }

void SdSymImpProp::read(const QJsonObject obj)
  {
  mAngle.read( QStringLiteral("SymImpAngle"), obj );
  mMirror.read( QStringLiteral("SymImpMirror"), obj );
  }
