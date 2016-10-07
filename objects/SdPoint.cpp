/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPoint.h"

SdPoint::SdPoint()
  {

  }




void SdPoint::write(const QString name, QJsonObject &obj) const
  {
  obj.insert( name, write() );
  }




void SdPoint::read(const QString name, const QJsonObject obj)
  {
  read( obj.value(name).toObject() );
  }




QJsonObject SdPoint::write() const
  {
  QJsonObject obj;
  obj.insert( QString("x"), QJsonValue(x) );
  obj.insert( QString("y"), QJsonValue(y) );
  return obj;
  }




void SdPoint::read(const QJsonObject obj)
  {
  x = obj.value( QString("x") );
  y = obj.value( QString("y") );
  }



SdAngle calcDirection90( SdPoint a, SdPoint b ) {
  if( abs(b.x - a.x) > abs(b.y - a.y) )
    //Приоритетная ось X
    return b.x > a.x ? da0 : da180;
  else
    //Приоритетная ось Y
    return b.y > a.y ? da90 : da270;
  }
