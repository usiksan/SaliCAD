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





void SdPointList::move(QSet<int> indexes, SdPoint offset)
  {
  for( int index : indexes ) {
    SdPoint p( at(index) );
    p.move(offset);
    setPoint( index, p );
    }
  }




void SdPointList::rotate(SdPoint center, SdAngle angle)
  {
  for( int index = 0; index < count(); index++ ) {
    SdPoint p( at(index) );
    p.rotate( center, angle );
    setPoint( index, p );
    }
  }




void SdPointList::mirror(SdPoint a, SdPoint b)
  {
  for( int index = 0; index < count(); index++ ) {
    SdPoint p( at(index) );
    p.mirror( a, b );
    setPoint( index, p );
    }
  }



QJsonArray SdPointList::write() const
  {
  QJsonArray array;
  for( int i = 0; i < count(); i++ )
    array.append( QJsonValue(get(i).write()) );
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
