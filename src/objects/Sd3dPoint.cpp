#include "Sd3dPoint.h"

#include <QJsonArray>



void Sd3dPoint::setAsMcm(int cx, int cy, int cz)
  {
  setX( intMcmToFloatMm( cx ) );
  setY( intMcmToFloatMm( cy ) );
  setZ( intMcmToFloatMm( cz ) );
  }

void Sd3dPoint::clear()
  {
  setX(0.0);
  setY(0.0);
  setZ(0.0);
  }

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
  Sd3dPoint tmp = (*this);
  (*this) = (*p);
  (*p) = tmp;
  }



QJsonObject Sd3dPoint::write() const
  {
  QJsonObject obj;
  obj.insert( QStringLiteral("x"), QJsonValue(x()) );
  obj.insert( QStringLiteral("y"), QJsonValue(y()) );
  obj.insert( QStringLiteral("z"), QJsonValue(z()) );
  return obj;
  }




void Sd3dPoint::read(const QJsonObject obj)
  {
  setX( obj.value( QStringLiteral("x") ).toDouble() );
  setY( obj.value( QStringLiteral("y") ).toDouble() );
  setZ( obj.value( QStringLiteral("z") ).toDouble() );
  }



QJsonArray sd3dPointListWrite(const Sd3dPointList &list)
  {
  QJsonArray array;
  for( auto const &pt : list ) {
    array.append( floatMmToIntMcm(pt.x()) );
    array.append( floatMmToIntMcm(pt.y()) );
    array.append( floatMmToIntMcm(pt.z()) );
    //array.append( pt.write() );
    }
  return array;
  }




void sd3dPointListRead(Sd3dPointList &list, const QJsonArray &array)
  {
  list.clear();
  Sd3dPoint pt;
  int i = 0;
  for( auto it = array.cbegin(); it != array.cend(); it++ ) {
    switch(i) {
      case 0 : pt.setX( intMcmToFloatMm(it->toInt()) ); break;
      case 1 : pt.setY( intMcmToFloatMm(it->toInt()) ); break;
      case 2 : pt.setZ( intMcmToFloatMm(it->toInt()) ); break;
      }
    i++;
    if( i > 2 ) {
      list.append( pt );
      i = 0;
      }
    }
  }
