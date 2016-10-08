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
#include <math.h>



void SdPoint::rotate(SdPoint origin, SdAngle angle)
  {
  SdPoint p;
  switch( angle ) {
    case da0   :
      p.setX( x() - origin.x() );
      p.setY( y() - origin.y() );
      break;
    case da90  :
      p.setX( -y() + origin.y() );
      p.setY(  x() - origin.x() );
      break;
    case da180 :
      p.setX( -x() + origin.x() );
      p.setY( -y() + origin.y() );
      break;
    case da270 :
      p.setX(  y() - origin.y() );
      p.setY( -x() + origin.x() );
      break;
    default :
      double dx = x() - origin.x(), dy = y() - origin.y();
      double sinAngle = sin(M_PI * angle), cosAngle = cos(M_PI * angle);
      setX( (int)(origin.x() + dx*cosAngle - dy*sinAngle) );
      setY( (int)(origin.y() + dx*sinAngle + dy*cosAngle) );
      return;
    }
  p.move( origin );
  set( p.x(), p.y() );
  }




void SdPoint::mirror(SdPoint origin)
  {
  setX( origin.x() - ( x() - origin.x() ) );
  }




void SdPoint::mirror(SdPoint a, SdPoint b)
  {
  /*Идея: переносим начало координат в точку а, поворачиваем плоскость до точки b
          таким образом ab образуют ось 0х, производим зеркализацию по y и делаем
          обратные преобразования
  */
  //Переносим
  move( a.complement() );
  //Поворачиваем
  SdAngle ang = b.getAngle(a);
  rotate( SdPoint(), ang.complement() );
  //Зеркализация
  setY( -y() );
  //Обратный поворот
  rotate( SdPoint(), ang );
  //Перенос
  move( a );
  }




SdAngle SdPoint::getAngle(SdPoint center) const
  {
  double dx = x() - center.x();
  double dy = y() - center.y();
  double radius = getDistance(center);
  double ang;
  if( radius ) {
    ang = asin( dy / radius ) * 90 / M_PI_2;
    //double angCos = acos( dx / radius ) * 90 / M_PI_2;
    if( dx < 0 ) ang = 180 - ang;
    if( dy < 0 ) ang = 360 + ang;
    }
  else ang = 0;
  return SdAngle((int)(ang*1000));
  }





double SdPoint::getDistance(SdPoint p) const
  {
  double dx = x() - p.x();
  double dy = y() - p.y();
  return sqrt( dx*dx + dy*dy );
  }




SdPoint SdPoint::convertImplement(SdPoint origin, SdPoint offset, SdAngle angle, bool mirror)
  {
  //Образовать точку и перенести начало координат в origin
  SdPoint p(x()-origin.x(),y()-origin.y());
  //Если требуется, произвести зеркальность
  if( mirror ) p.setX( -p.x() );
  //Поворот
  p.rotate( SdPoint(), angle );
  p.move( offset );
  return p;
  }




SdPoint SdPoint::unConvertImplement(SdPoint origin, SdPoint offset, SdAngle angle, bool mirror)
  {
  //Образовать точку и перенести начало координат в offset
  SdPoint p(x()-offset.x(),y()-offset.y());
  //Поворот
  p.rotate( SdPoint(), angle.complement() );
  //Если требуется, произвести зеркальность
  if( mirror ) p.setX( -p.x() );
  //Сместить на origin
  p.move( origin );
  return p;
  }




bool SdPoint::isOnCircle(SdPoint center, int radius, int delta) const
  {
  //Точка на окружности, если расстояние отсюда до центра равно радиусу
  return abs( radius - getDistance(center) ) <= delta;
  }

bool SdPoint::isOnArc(SdPoint center, SdPoint start, SdPoint stop, int delta) const
  {
  if( isOnCircle( center, center.getDistance(start), delta )  ) {
    //Расстояние отсюда до центра дуги равно радиусу дуги, проверяем дальше
    SdAngle anStart = start.getAngle(center);
    SdAngle anStop  = stop.getAngle(center);
    SdAngle cur     = getAngle(center);
    return cur - anStart < anStop - anStart;
    }
  return false;
  }




bool SdPoint::isLeftHigh(SdPoint a) const
  {
  return x() < a.x() || (x() == a.x() && y() > a.y());
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
  obj.insert( QString("x"), QJsonValue(x()) );
  obj.insert( QString("y"), QJsonValue(y()) );
  return obj;
  }




void SdPoint::read(const QJsonObject obj)
  {
  setX( obj.value( QString("x") ).toInt() );
  setY( obj.value( QString("y") ).toInt() );
  }



SdAngle calcDirection90( SdPoint a, SdPoint b ) {
  if( abs(b.x() - a.x()) > abs(b.y() - a.y()) )
    //Приоритетная ось X
    return b.x() > a.x() ? da0 : da180;
  else
    //Приоритетная ось Y
    return b.y() > a.y() ? da90 : da270;
  }
