/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
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

    QJsonArray write();
    void       write( const QString name, QJsonObject &obj );

    void       read( const QJsonArray array );
    void       read( const QString name, const QJsonObject obj );
  }

#endif // SDPOINTLIST_H
