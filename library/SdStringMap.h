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

//Write and read string map to/from json
void sdStringMapWrite( const QString name, const SdStringMap &table, QJsonObject &obj );
void sdStringMapRead( const QString name, SdStringMap &table, const QJsonObject &obj );

//Union two string maps. All keys from source map append and replace keys in dest map.
void sdStringMapUnion( const SdStringMap &src, SdStringMap &dst );


#endif // SDSTRINGMAP_H
