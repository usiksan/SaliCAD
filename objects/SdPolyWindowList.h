/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDPOLYWINDOWLIST_H
#define SDPOLYWINDOWLIST_H

#include "SdPolyWindow.h"
#include <QList>

class SdPolyWindowList : public QList<SdPolyWindow>
  {
  public:
    SdPolyWindowList() {}

    QJsonArray write();
    void       write( const QString name, QJsonObject &obj );

    void       read( const QJsonArray array );
    void       read( const QString name, const QJsonObject obj );
  };

#endif // SDPOLYWINDOWLIST_H
