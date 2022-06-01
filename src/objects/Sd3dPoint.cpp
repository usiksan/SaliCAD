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




void Sd3dPoint::swap(Sd3dPoint *p)
  {
  Sd3dPoint tmp = (*this);
  (*this) = (*p);
  (*p) = tmp;
  }










void json3dPointList(SvJsonWriter &js, const QString &key, const Sd3dPointList &list)
  {
  QJsonArray array;
  for( auto const &pt : list ) {
    array.append( floatMmToIntMcm(pt.x()) );
    array.append( floatMmToIntMcm(pt.y()) );
    array.append( floatMmToIntMcm(pt.z()) );
    }
  js.object().insert( key, array );
  }




void json3dPointList(const SvJsonReader &js, const QString &key, Sd3dPointList &list)
  {
  QJsonArray array = js.object().value( key ).toArray();
  list.clear();
  Sd3dPoint pt;
  int i = 0;
  for( auto it = array.constBegin(); it != array.constEnd(); it++ ) {
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
