/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPolyWindowList.h"
#include <QJsonArray>
#include <QPolygonF>


void SdPolyWindowList::appendRegion(const QPolygonF &pgn)
  {
  //We append only 4 vertex regions
  if( pgn.count() == 4 ) {
    SdPolyWindow win( pgn );
    append( win );
    }
  }




void SdPolyWindowList::appendCircle(SdPoint center, int radius)
  {
  SdPolyWindow win( center, radius );
  append( win );
  }




QJsonArray SdPolyWindowList::write() const
  {
  QJsonArray arr;
  for( int i = 0; i < count(); i++ )
    arr.append( at(i).write() );
  return arr;
  }




void SdPolyWindowList::write(const QString name, QJsonObject &obj) const
  {
  obj.insert( name, write() );
  }




void SdPolyWindowList::read(const QJsonArray array)
  {
  clear();
  SdPolyWindow win;
  for( int i = 0; i < array.count(); i++ ) {
    win.read( array.at(i).toObject() );
    append( win );
    }
  }

void SdPolyWindowList::read(const QString name, const QJsonObject obj)
  {
  read( obj.value(name).toArray() );
  }

