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
#include "SdObjectFactory.h"
#include "SdPItemSymbol.h"
#include "SdPItemPart.h"
#include "SdPItemComponent.h"
#include "SdPItemSheet.h"
#include "SdPItemPlate.h"
#include "SdContainerSheetNet.h"
#include "SdGraphArea.h"
#include "SdGraphLinearArc.h"
#include "SdGraphLinearLine.h"
#include "SdGraphLinearRect.h"
#include "SdGraphLinearRectFilled.h"
#include "SdGraphLinearCircle.h"
#include "SdGraphLinearCircleFilled.h"
#include "SdGraphLinearRegion.h"
#include "SdGraphText.h"
#include "SdGraphIdent.h"
#include "SdGraphSymPin.h"
#include "SdGraphPartPin.h"
#include "SdGraphIdent.h"
#include "SdGraphSymImp.h"
#include "SdGraphPartImp.h"
#include "SdGraphWiringWire.h"
#include "SdGraphWireName.h"
#include "SdSection.h"
#include "SdPartVariant.h"
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




void SdObject::setParent(SdContainer *parent)
  {
  Q_ASSERT( mParent == nullptr );
  mParent = parent;
  }




void SdObject::attach(SdUndo *undo)
  {
  Q_UNUSED(undo)
  }




void SdObject::detach(SdUndo *undo)
  {
  Q_UNUSED(undo)
  }




bool SdObject::isUsed(SdObject *obj) const
  {
  Q_UNUSED(obj)
  return false;
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
  const SdProjectItem *item = dynamic_cast<const SdProjectItem*>(this);
  if( item && (item->getClass() & (dctComponent | dctPart | dctSymbol)) )
    SdObjectFactory::insertObject( item, obj );
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
  if( type == QStringLiteral(SD_TYPE_ARC)           ) return new SdGraphLinearArc();
  if( type == QStringLiteral(SD_TYPE_LINE)          ) return new SdGraphLinearLine();
  if( type == QStringLiteral(SD_TYPE_RECT)          ) return new SdGraphLinearRect();
  if( type == QStringLiteral(SD_TYPE_RECT_FILLED)   ) return new SdGraphLinearRectFilled();
  if( type == QStringLiteral(SD_TYPE_CIRCLE)        ) return new SdGraphLinearCircle();
  if( type == QStringLiteral(SD_TYPE_CIRCLE_FILLED) ) return new SdGraphLinearCircleFilled();
  if( type == QStringLiteral(SD_TYPE_REGION)        ) return new SdGraphLinearRegion();
  if( type == QStringLiteral(SD_TYPE_TEXT)          ) return new SdGraphText();
  if( type == QStringLiteral(SD_TYPE_IDENT)         ) return new SdGraphIdent();
  if( type == QStringLiteral(SD_TYPE_SYM_PIN)       ) return new SdGraphSymPin();
  if( type == QStringLiteral(SD_TYPE_PART_PIN)      ) return new SdGraphPartPin();
  if( type == QStringLiteral(SD_TYPE_AREA)          ) return new SdGraphArea();
  if( type == QStringLiteral(SD_TYPE_IDENT)         ) return new SdGraphIdent();
  if( type == QStringLiteral(SD_TYPE_SYM_IMP)       ) return new SdGraphSymImp();
  if( type == QStringLiteral(SD_TYPE_PART_IMP)      ) return new SdGraphPartImp();
  if( type == QStringLiteral(SD_TYPE_WIRE)          ) return new SdGraphWiringWire();
  if( type == QStringLiteral(SD_TYPE_WIRE_NAME)     ) return new SdGraphWireName();

  if( type == QStringLiteral(SD_TYPE_SHEET_NET)     ) return new SdContainerSheetNet();

  if( type == QStringLiteral(SD_TYPE_SYMBOL)        ) return new SdPItemSymbol();
  if( type == QStringLiteral(SD_TYPE_PART)          ) return new SdPItemPart();
  if( type == QStringLiteral(SD_TYPE_SHEET)         ) return new SdPItemSheet();
  if( type == QStringLiteral(SD_TYPE_PLATE)         ) return new SdPItemPlate();
  if( type == QStringLiteral(SD_TYPE_COMPONENT)     ) return new SdPItemComponent();

  if( type == QStringLiteral(SD_TYPE_SECTION)       ) return new SdSection();
  if( type == QStringLiteral(SD_TYPE_PART_VARIANT)  ) return new SdPartVariant();

  if( type == QStringLiteral(SD_TYPE_PROJECT)       ) return new SdProject();
  return 0;
  }



