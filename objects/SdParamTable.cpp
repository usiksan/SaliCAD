/*
  Проект "SaliBrick"
    Визуальное программирование микроконтроллеров
  Автор
    Сибилев А.С.
  Описание
*/
#include "SdParamTable.h"


void sdParamWrite(const QString name, const SdParamTable &table, QJsonObject &obj)
  {
  obj.insert( name, QJsonValue( QJsonObject::fromVariantMap(table) ) );
  }


void sdParamRead(const QString name, SdParamTable &table, const QJsonObject &obj)
  {
  table = obj.value(name).toObject().toVariantMap();
  }
