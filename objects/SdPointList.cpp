/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Point list for rigions, polygons, areas and s.o.
*/

#include "SdPointList.h"


enum { INSIDE, OUTSIDE, BOUNDARY };         // положение точки
//     ВНУТРИ, ВНЕ,     НА ГРАНИЦЕ
enum { TOUCHING, CROSSING, INESSENTIAL };   // положение ребра
//     КАСАТЕЛbНОЕ, ПЕРЕСЕКАЮЩЕЕ, НЕСУЩЕСТВЕННОЕ

inline int edgeType( SdPoint a, SdPoint v, SdPoint w )
  {
  switch( a.classify( v, w ) ) {
    case SdPoint::LEFT :
      return ((v.y() < a.y()) && (a.y() <= w.y())) ? CROSSING : INESSENTIAL;
    case SdPoint::RIGHT:
      return ((w.y() < a.y()) && (a.y() <= v.y())) ? CROSSING : INESSENTIAL;
    case SdPoint::BETWEEN:
    case SdPoint::ORIGIN:
    case SdPoint::DESTINATION:
      return TOUCHING;
    case SdPoint::BEYOND :
    case SdPoint::BEHIND :
      return INESSENTIAL;
    }
  }


bool SdPointList::isPointInside(SdPoint p)
  {
  if( count() < 3 )
    return false;
  int parity = 0;
  for( int i = 0; i < count(); i++ ) {
    switch( edgeType( p, at(i), at( i == 0 ? count()-1 : i - 1 ) )  ) {
      case TOUCHING:
        return true;
      case CROSSING:
        parity = 1 - parity;
      }
    }
  return parity;
  }

QJsonArray SdPointList::write() const
  {
  QJsonArray array;
  for( int i = 0; i < count(); i++ )
    array.append( QJsonValue(at(i).write()) );
  return array;
  }




void SdPointList::write(const QString name, QJsonObject &obj) const
  {
  obj.insert( name, QJsonValue( write() ) );
  }



void SdPointList::read(const QJsonArray array)
  {
  clear();
  SdPoint p;
  for( int i = 0; i < array.count(); i++ ) {
    p.read( array.at(i).toObject() );
    append(p);
    }
  }




void SdPointList::read(const QString name, const QJsonObject obj)
  {
  read( obj.value(name).toArray() );
  }
