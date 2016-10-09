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




void SdProject::insert(SdProjectItem *item)
  {
  if( !isContains(item->getTitle()) ) {
    mChildList.append( item );
    mItemMap.insert( item->getTitle(), item );
    item->setParent( this );
    mDirty = true;
    SdPulsar::pulsar->emitInsertItem( item );
    }
  }




SdObject *SdProject::copy()
  {
  return 0;
  }




void SdProject::writeObject(QJsonObject &obj) const
  {
  SdContainer::writeObject( obj );
  }




void SdProject::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdContainer::readObject( map, obj );

  //Fill map
  forEach( dctProjectItems, [this](SdObject *obj) -> bool {
    SdProjectItem *it = dynamic_cast<SdProjectItem*>(obj);
    if( it ) mItemMap.insert( it->getTitle(), it );
    return true;
    } );

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


