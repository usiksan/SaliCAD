/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPropSymImp.h"


void SdPropSymImp::operator =(const SdPropSymImp &sour)
  {
  mAngle  = sour.mAngle;
  mMirror = sour.mMirror;
  }




void SdPropSymImp::append(const SdPropSymImp &sour)
  {
  mAngle.append( sour.mAngle );
  mMirror.append( sour.mMirror );
  }




void SdPropSymImp::clear()
  {
  mAngle.clear();
  mMirror.clear();
  }




bool SdPropSymImp::match(const SdPropSymImp &prop)
  {
  return mAngle.match( prop.mAngle ) && mMirror.match( prop.mMirror );
  }




void SdPropSymImp::write(QJsonObject &obj) const
  {
  mAngle.write( QStringLiteral("SymImpAngle"), obj );
  mMirror.write( QStringLiteral("SymImpMirror"), obj );
  }




void SdPropSymImp::read(const QJsonObject obj)
  {
  mAngle.read( QStringLiteral("SymImpAngle"), obj );
  mMirror.read( QStringLiteral("SymImpMirror"), obj );
  }



void SdPropSymImp::saveState(SdPropSymImpState *dst)
  {
  dst->mAngle  = mAngle.getValue();
  dst->mMirror = mMirror.getValue();
  }




void SdPropSymImp::swapState(SdPropSymImpState *src)
  {
  src->mAngle  = mAngle.swap( src->mAngle );
  src->mMirror = mMirror.swap( src->mMirror );
  }

