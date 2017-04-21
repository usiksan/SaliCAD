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
#include "SdPItemSymbol.h"
#include "SdIds.h"
#include "SdGraphLinearLine.h"
#include "SdGraphLinearRect.h"
#include "SdGraphLinearRectFilled.h"
#include "SdGraphLinearCircle.h"
#include "SdGraphLinearCircleFilled.h"
#include "SdGraphLinearRegion.h"
#include "SdGraphText.h"
#include <QJsonValue>
#include <QDebug>

SdObject::SdObject() :
  mParent(0),
  mDeleted(false)
  {

  }



QString SdObject::getId() const
  {
  return QString::number( (quintptr)this, 16 );
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




void SdObject::attach(SdUndo *undo)
  {
  Q_UNUSED(undo)
  }




void SdObject::detach(SdUndo *undo)
  {
  Q_UNUSED(undo)
  }



SdObject *SdObject::copy()
  {
  SdObject *obj = build( getType() );
  obj->cloneFrom( this );
  return obj;
  }



void SdObject::cloneFrom( const SdObject *src )
  {
  if( getType() != src->getType() ) {
    qDebug() << Q_FUNC_INFO << "Illegal clone source";
    throw( QString("Illegal clone source") );
    }
  }




void SdObject::writeObject(QJsonObject &obj) const
  {
  Q_UNUSED(obj)
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




void SdObject::writePtr( const SdObject *ptr, const QString name, QJsonObject &obj)
  {
  QJsonObject sub;
  if( ptr ) {
    sub.insert( QStringLiteral(SDKO_ID), QJsonValue( ptr->getId() ) );
    sub.insert( QStringLiteral(SDKO_TYPE), QJsonValue( ptr->getType() ) );
    }
  else
    sub.insert( QStringLiteral(SDKO_ID), QJsonValue( QString() ) );
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
  QString id = obj.value( QStringLiteral(SDKO_ID) ).toString();

  //If id equals zero then no object
  if( id.isEmpty() )
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
  if( type == QString(SD_TYPE_PROJECT)       ) return new SdProject();
  if( type == QString(SD_TYPE_SYMBOL)        ) return new SdPItemSymbol();
  if( type == QString(SD_TYPE_LINE)          ) return new SdGraphLinearLine();
  if( type == QString(SD_TYPE_RECT)          ) return new SdGraphLinearRect();
  if( type == QString(SD_TYPE_RECT_FILLED)   ) return new SdGraphLinearRectFilled();
  if( type == QString(SD_TYPE_CIRCLE)        ) return new SdGraphLinearCircle();
  if( type == QString(SD_TYPE_CIRCLE_FILLED) ) return new SdGraphLinearCircleFilled();
  if( type == QString(SD_TYPE_TEXT)          ) return new SdGraphText();
  return 0;
  }



