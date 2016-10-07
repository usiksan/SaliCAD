/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdObject.h"
#include <QJsonValue>

SdObject::SdObject()
  {

  }


QJsonObject SdObject::write()
  {
  QJsonObject obj = writePtr();
  writeObject( obj );
  return obj;
  }

QJsonObject SdObject::writePtr()
  {
  QJsonObject obj;
  obj.insert( QString(SDKO_ID), QJsonValue( getId() ) );
  obj.insert( QString(SDKO_TYPE), QJsonValue( getType() ) );
  return obj;
  }





SdObject *SdObject::read(SdObjectMap *map, const QJsonObject obj)
  {
  SdObject *r = readPtr( map, obj );
  if( r )
    r->readObject( map, obj );

  return r;
  }




SdObject *SdObject::readPtr(SdObjectMap *map, const QJsonObject obj)
  {
  //Get object id
  int id = obj.value( QString(SDKO_ID) ).toInt();

  //If id equals zero then no object
  if( id == 0 )
    return 0;

  //Check if object already in the map
  if( map->contains(id) )
    return map->value(id);

  //Build new object
  SdObject *r = build( obj.value( QString(SDKO_TYPE) ).toInt() );
  //Register new object in the map
  map->insert( id, r );
  //and return new object
  return r;
  }





SdObject *SdObject::build(int type)
  {
  switch( type ) {
    default : return 0;
    }
  }



