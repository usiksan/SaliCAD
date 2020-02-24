#include "SdPoint3d.h"




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
