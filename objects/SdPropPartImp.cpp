/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPropPartImp.h"


void SdPropPartImp::operator =(const SdPropPartImp &sour)
  {
  mAngle  = sour.mAngle;
  mMirror = sour.mMirror;
  mSide   = sour.mSide;
  }

void SdPropPartImp::append(const SdPropPartImp &sour)
  {
  mAngle.append( sour.mAngle );
  mMirror.append( sour.mMirror );
  mSide.append( sour.mSide );
  }

void SdPropPartImp::clear()
  {
  mAngle.clear();
  mMirror.clear();
  mSide.clear();
  }

bool SdPropPartImp::match(const SdPropPartImp &prop)
  {
  return mAngle.match( prop.mAngle ) && mMirror.match( prop.mMirror ) && mSide.match( prop.mSide );
  }

void SdPropPartImp::write(QJsonObject &obj)
  {
  mAngle.write( QStringLiteral("PartImpAngle"), obj );
  mMirror.write( QStringLiteral("PartImpMirror"), obj );
  mSide.write( QStringLiteral("PartImpSide"), obj );
  }

void SdPropPartImp::read(const QJsonObject obj)
  {
  mAngle.read( QStringLiteral("PartImpAngle"), obj );
  mMirror.read( QStringLiteral("PartImpMirror"), obj );
  mSide.read( QStringLiteral("PartImpSide"), obj );
  }

