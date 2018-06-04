/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  QMap<QString,QString> with utils
*/
#ifndef SDSTRINGMAP_H
#define SDSTRINGMAP_H

#include <QString>
#include <QMap>
#include <QJsonObject>

typedef QMap<QString,QString> SdStringMap;

void sdStringMapWrite( const QString name, const SdStringMap &table, QJsonObject &obj );
void sdStringMapRead( const QString name, SdStringMap &table, const QJsonObject &obj );


#endif // SDSTRINGMAP_H
