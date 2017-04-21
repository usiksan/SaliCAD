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
