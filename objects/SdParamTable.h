/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Text parametrs table
*/
#ifndef SDPARAMTABLE_H
#define SDPARAMTABLE_H

#include <QVariantMap>
#include <QString>
#include <QJsonObject>

typedef QVariantMap SdParamTable;

void sdParamWrite( const QString name, const SdParamTable &table, QJsonObject &obj );
void sdParamRead( const QString name, SdParamTable &table, const QJsonObject &obj );

#endif // SDPARAMTABLE_H
