#include "Sd3dPoint.h"

#include <QJsonArray>


inline double intMcmToFloatM( int v )
  {
  //Convert to double
  double d = v;
  //Convert mcm to meter
  return d / 1000000.0;
  }

void Sd3dPoint::setAsMcm(int cx, int cy, int cz)
  {
  setX( intMcmToFloatM( cx ) );
  setY( intMcmToFloatM( cy ) );
  setZ( intMcmToFloatM( cz ) );
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
  obj.insert( QStringLiteral("y"), QJsonValue(z()) );
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
    array.append( pt.write() );
    }
  return array;
  }




void sd3dPointListRead(Sd3dPointList &list, const QJsonArray &array)
  {
  list.clear();
  Sd3dPoint pt;
  for( auto it = array.cbegin(); it != array.cend(); it++ ) {
    pt.read( it->toObject() );
    list.append( pt );
    }
  }
