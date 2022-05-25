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




void SdPointList::rotate(SdPoint center, SdPropAngle angle)
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

