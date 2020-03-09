#include "Sd3dPoint.h"




void Sd3dPoint::write(const QString name, QJsonObject &obj) const
  {
  obj.insert( name, write() );
  }



void Sd3dPoint::read(const QString name, const QJsonObject obj)
  {
  read( obj.value(name).toObject() );
  }




void Sd3dPoint::swap(Sd3dPoint *p)
  {
  qSwap( mX, p->mX );
  qSwap( mY, p->mY );
  qSwap( mZ, p->mZ );
  }



QJsonObject Sd3dPoint::write() const
  {
  QJsonObject obj;
  obj.insert( QStringLiteral("x"), QJsonValue(mX) );
  obj.insert( QStringLiteral("y"), QJsonValue(mY) );
  obj.insert( QStringLiteral("y"), QJsonValue(mZ) );
  return obj;
  }




void Sd3dPoint::read(const QJsonObject obj)
  {
  mX = obj.value( QStringLiteral("x") ).toInt();
  mY = obj.value( QStringLiteral("y") ).toInt();
  mZ = obj.value( QStringLiteral("z") ).toInt();
  }
