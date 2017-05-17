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

    void       move( QSet<int> indexes, SdPoint offset );
    void       rotate(SdPoint center, SdAngle angle);
    void       mirror(SdPoint a, SdPoint b);

    QJsonArray write() const;
    void       write( const QString name, QJsonObject &obj ) const;

    void       read( const QJsonArray array );
    void       read( const QString name, const QJsonObject obj );
  };

#endif // SDPOINTLIST_H
