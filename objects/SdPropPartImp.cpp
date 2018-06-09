/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part implementation properties
*/

#include "SdPropPartImp.h"


void SdPropPartImp::operator =(const SdPropPartImp &sour)
  {
  mAngle  = sour.mAngle;
  mSide   = sour.mSide;
  }



void SdPropPartImp::append(const SdPropPartImp &sour)
  {
  mAngle.append( sour.mAngle );
  mSide.append( sour.mSide );
  }




void SdPropPartImp::clear()
  {
  mAngle.clear();
  mSide.clear();
  }




bool SdPropPartImp::match(const SdPropPartImp &prop)
  {
  return mAngle.match( prop.mAngle ) && mSide.match( prop.mSide );
  }




void SdPropPartImp::write(QJsonObject &obj) const
  {
  mAngle.write( QStringLiteral("PartImpAngle"), obj );
  mSide.write( QStringLiteral("PartImpSide"), obj );
  }




void SdPropPartImp::read(const QJsonObject obj)
  {
  mAngle.read( QStringLiteral("PartImpAngle"), obj );
  mSide.read( QStringLiteral("PartImpSide"), obj );
  }




void SdPropPartImp::saveState(SdPropPartImpState *dst)
  {
  dst->mAngle  = mAngle.getValue();
  dst->mSide   = mSide.getValue();
  }




void SdPropPartImp::swapState(SdPropPartImpState *src)
  {
  src->mAngle  = mAngle.swap( src->mAngle );
  src->mSide   = mSide.swap( src->mSide );
  }

