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

#include <QTransform>



void SdPointList::transform(QSet<int> indexes, const QTransform &map)
  {
  for( int index : indexes ) {
    setPoint( index, map.map(at(index)) );
    }
  }



void SdPointList::transform(const QTransform &map)
  {
  for( int index = 0; index < count(); index++ ) {
    setPoint( index, map.map( at(index) ) );
    }

  }







//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void SdPointList::json( const QString &key, SvJsonWriter &js) const
  {
  QJsonArray array;
  for( int i = 0; i < count(); i++ ) {
    SvJsonWriter sjs;
    get(i).json( sjs );
    array.append( QJsonValue(sjs.object()) );
    }
  js.object().insert( key, array );
  }





//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void SdPointList::json(const QString &key, const SvJsonReader &js)
  {
  clear();
  SdPoint p;
  QJsonArray array = js.object().value(key).toArray();
  for( int i = 0; i < array.count(); i++ ) {
    SvJsonReader sjs( array.at(i).toObject() );
    p.json( sjs );
    append(p);
    }
  }

