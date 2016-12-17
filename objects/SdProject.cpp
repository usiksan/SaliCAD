/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdProject.h"
#include "SdProjectItem.h"
#include "SdPulsar.h"
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QByteArray>

SdProject::SdProject() :
  mDirty(false)
  {
  }




SdProject::~SdProject()
  {
  }




QString SdProject::getType() const
  {
  return QString(SD_TYPE_PROJECT);
  }




quint64 SdProject::getClass() const
  {
  return dctProject;
  }




void SdProject::writeObject(QJsonObject &obj) const
  {
  SdContainer::writeObject( obj );
  obj.insert( QStringLiteral("Properties"), mProperties );
  }




void SdProject::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdContainer::readObject( map, obj );
  mProperties = obj.value( QStringLiteral("Properties") ).toObject();

  //Fill map
  fillMap();
  }




SdProject *SdProject::load(const QString fname)
  {
  QFile file(fname);
  if( file.open(QIODevice::ReadOnly) ) {
    QJsonDocument doc = QJsonDocument::fromJson( file.readAll(), 0 );
    if( !doc.isEmpty() ) {
      SdObjectMap map;
      return only<SdProject>( SdObject::read( &map, doc.object() ) );
      }
    }
  return 0;
  }




bool SdProject::save(const QString fname)
  {
  QFile file(fname);
  if( file.open(QIODevice::WriteOnly) ) {
    QJsonDocument doc( write() );
    file.write( doc.toJson() );
    mDirty = false;
    return true;
    }
  return false;
  }




void SdProject::cloneFrom(const SdObject *src)
  {
  SdContainer::cloneFrom(src);
  mDirty = true;
  const SdProject *sour = dynamic_cast<const SdProject*>(src);
  mProperties = sour->mProperties;
  fillMap();
  }




void SdProject::insertChild(SdObject *child)
  {
  SdProjectItem *item = dynamic_cast<SdProjectItem*>( child );
  if( item && !isContains(item->getTitle()) ) {
    SdContainer::insertChild( child );
    mItemMap.insert( item->getTitle(), item );
    mDirty = true;
    SdPulsar::pulsar->emitInsertItem( item );
    }
  }




void SdProject::undoInsertChild(SdObject *child)
  {
  SdProjectItem *item = dynamic_cast<SdProjectItem*>( child );
  if( item && item->getParent() == this ) {
    SdPulsar::pulsar->emitRemoveItem( item );
    mItemMap.remove( item->getTitle() );
    mDirty = true;
    SdContainer::undoInsertChild( child );
    }
  }




void SdProject::deleteChild(SdObject *child)
  {
  SdProjectItem *item = dynamic_cast<SdProjectItem*>( child );
  if( item && item->getParent() == this ) {
    SdPulsar::pulsar->emitRemoveItem( item );
    mItemMap.remove( item->getTitle() );
    mDirty = true;
    SdContainer::deleteChild( child );
    }
  }




void SdProject::undoDeleteChild(SdObject *child)
  {
  SdProjectItem *item = dynamic_cast<SdProjectItem*>( child );
  if( item && item->getParent() == this ) {
    SdContainer::undoDeleteChild( child );
    mItemMap.insert( item->getTitle(), item );
    mDirty = true;
    SdPulsar::pulsar->emitInsertItem( item );
    }
  }




void SdProject::fillMap()
  {
  mItemMap.clear();
  forEach( dctProjectItems, [this](SdObject *obj) -> bool {
    SdProjectItem *it = dynamic_cast<SdProjectItem*>(obj);
    if( it ) mItemMap.insert( it->getTitle(), it );
    return true;
    } );
  }
