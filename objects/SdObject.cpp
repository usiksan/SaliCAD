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
#include "SdContainer.h"
#include "SdProject.h"
#include <QJsonValue>

SdObject::SdObject() :
  mParent(0)
  {

  }

SdContainer *SdObject::getRoot() const
  {
  //if( mParent == 0 ) return dynamic_cast<SdContainer*>( this );
  SdContainer *p = mParent;
  while(p) {
    if( p->mParent == 0 ) return p;
    p = p->mParent;
    }
  return 0;
  }


QJsonObject SdObject::write() const
  {
  QJsonObject obj;
  obj.insert( QStringLiteral(SDKO_ID), QJsonValue( getId() ) );
  obj.insert( QStringLiteral(SDKO_TYPE), QJsonValue( getType() ) );
  writePtr( mParent, QStringLiteral("Parent"), obj );
  writeObject( obj );
  return obj;
  }

void SdObject::writePtr(const SdObject *ptr, const QString name, QJsonObject &obj)
  {
  QJsonObject sub;
  if( ptr ) {
    sub.insert( QStringLiteral(SDKO_ID), QJsonValue( ptr->getId() ) );
    sub.insert( QStringLiteral(SDKO_TYPE), QJsonValue( ptr->getType() ) );
    }
  else
    sub.insert( QStringLiteral(SDKO_ID), QJsonValue( (int)0 ) );
  obj.insert( name, sub );
  }

void SdObject::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  mParent = dynamic_cast<SdContainer*>( readPtr(QStringLiteral("Parent"), map, obj ) );
  }





SdObject *SdObject::read(SdObjectMap *map, const QJsonObject obj)
  {
  SdObject *r = readPtr( map, obj );
  if( r ) {
    r->readObject( map, obj );
    }

  return r;
  }




SdObject *SdObject::readPtr(SdObjectMap *map, const QJsonObject obj)
  {
  //Get object id
  int id = obj.value( QStringLiteral(SDKO_ID) ).toInt();

  //If id equals zero then no object
  if( id == 0 )
    return 0;

  //Check if object already in the map
  if( map->contains(id) )
    return map->value(id);

  //Build new object
  SdObject *r = build( obj.value( QStringLiteral(SDKO_TYPE) ).toString() );
  //Register new object in the map
  map->insert( id, r );
  //and return new object
  return r;
  }

SdObject *SdObject::readPtr(const QString name, SdObjectMap *map, const QJsonObject obj)
  {
  return readPtr( map, obj.value(name).toObject() );
  }





SdObject *SdObject::build(QString type)
  {
  if( type == QString(SD_TYPE_PROJECT) ) return new SdProject();
  return 0;
  }



