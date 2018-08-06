/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdStringMap.h"


void sdStringMapWrite(const QString name, const SdStringMap &table, QJsonObject &obj)
  {
  //Destignation json object
  QJsonObject dst;
  //For each pair write json value
  for( auto i = table.constBegin(); i != table.constEnd(); i++ )
    dst.insert( i.key(), i.value() );
  obj.insert( name, dst );
  }




void sdStringMapRead(const QString name, SdStringMap &table, const QJsonObject &obj)
  {
  QJsonObject src = obj.value( name ).toObject();
  table.clear();
  for( auto i = src.constBegin(); i != src.constEnd(); i++ )
    table.insert( i.key(), i.value().toString() );
  }




//Union two string maps. All keys from source map append and replace keys in dest map.
void sdStringMapUnion(const SdStringMap &src, SdStringMap &dst)
  {
  for( auto i = src.cbegin(); i != src.cend(); i++ )
    dst.insert( i.key(), i.value() );
  }
