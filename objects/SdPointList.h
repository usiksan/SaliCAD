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
#include <QList>
#include <QJsonArray>
#include <QJsonObject>

class SdPointList : public QList<SdPoint>
  {
  public:
    SdPointList() {}
    SdPointList( const SdPointList &src ) : QList<SdPoint>( src ) {}

    QJsonArray write();
    void       write( const QString name, QJsonObject &obj );

    void       read( const QJsonArray array );
    void       read( const QString name, const QJsonObject obj );
  };

#endif // SDPOINTLIST_H
