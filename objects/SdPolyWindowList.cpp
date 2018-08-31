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
#include "SdUtil.h"

#include <QJsonArray>
#include <QPolygonF>


//Reset windows list
void SdPolyWindowList::reset(SdPointList *poly)
  {
  clear();
  mPolygon = poly;
  }



void SdPolyWindowList::appendRegion(const QPolygonF &pgn)
  {
  //We append only 4 vertex regions
  if( pgn.count() == 4 ) {
    if( mPolygon && pgn.intersects( *mPolygon ) ) {
      SdPolyWindow win( pgn );
      append( win );
      }
    }
  }




void SdPolyWindowList::appendCircle(SdPoint center, int radius)
  {
  QPolygonF pgn = SdUtil::octagon( center.x(), center.y(), radius );
  if( mPolygon && pgn.intersects( *mPolygon ) ) {
    SdPolyWindow win( center, radius );
    append( win );
    }
  }




//Test point
bool SdPolyWindowList::containsPoint(SdPoint p) const
  {
  for( const SdPolyWindow &win : *this )
    if( win.containsPoint(p) ) return true;
  return false;
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

