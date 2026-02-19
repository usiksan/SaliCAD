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

#ifndef SDPOINTLIST_H
#define SDPOINTLIST_H

#include "SdPoint.h"
#include <QPolygon>
#include <QJsonArray>
#include <QJsonObject>
#include <QSet>

class SdPointList : public QPolygon
  {
  public:
    SdPointList() {}
    SdPointList( const SdPointList &src ) : QPolygon( src ) {}

    SdPoint    get(int i) const { return SdPoint( at(i) ); }

    void       transform( QSet<int> indexes, const QTransform &map );
    void       transform( const QTransform &map );


    //!
    //! \brief json Overloaded function to write object content into json writer
    //! \param js   Json writer
    //!
    void       json( const QString &key, SvJsonWriter &js ) const;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //! \param js   Json reader
    //!
    void       json( const QString &key, const SvJsonReader &js);

  };

#endif // SDPOINTLIST_H
