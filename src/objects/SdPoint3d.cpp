#include "SdPoint3d.h"




void SdPoint3d::write(const QString name, QJsonObject &obj) const
  {
  obj.insert( name, write() );
  }



void SdPoint3d::read(const QString name, const QJsonObject obj)
  {
  read( obj.value(name).toObject() );
  }




void SdPoint3d::swap(SdPoint3d *p)
  {
  qSwap( mX, p->mX );
  qSwap( mY, p->mY );
  qSwap( mZ, p->mZ );
  }



QJsonObject SdPoint3d::write() const
  {
  QJsonObject obj;
  obj.insert( QStringLiteral("x"), QJsonValue(mX) );
  obj.insert( QStringLiteral("y"), QJsonValue(mY) );
  obj.insert( QStringLiteral("y"), QJsonValue(mZ) );
  return obj;
  }




void SdPoint3d::read(const QJsonObject obj)
  {
  mX = obj.value( QStringLiteral("x") ).toInt();
  mY = obj.value( QStringLiteral("y") ).toInt();
  mZ = obj.value( QStringLiteral("z") ).toInt();
  }
