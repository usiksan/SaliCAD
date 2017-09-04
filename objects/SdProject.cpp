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
#include "SdPItemPlate.h"
#include "SdPItemSheet.h"
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QByteArray>
#include <QDebug>

SdProject::SdProject() :
  mDirty(false),
  mNetIndex(0)
  {
  }




SdProject::~SdProject()
  {
  }




//Return default plate and if none - create new one
SdPItemPlate *SdProject::getDefaultPlate()
  {
  //At first try to find plate in child list
  for( SdObject *obj : mChildList ) {
    if( !obj->isDeleted() && obj->getClass() == dctPlate ) {
      //Plate found. Return it
      return dynamic_cast<SdPItemPlate*>(obj);
      }
    }

  //Plate not found. Create new one
  SdPItemPlate *plate = new SdPItemPlate();
  plate->setTitle( QStringLiteral("Default plate") );
  insertChild( plate, &mUndo );
  return plate;
  }




//Return object of project for given object
SdProjectItem *SdProject::getProjectsItem(SdProjectItem *item)
  {
  //If source is null then return null also
  if( item == nullptr ) return item;
  //First try find equal object
  SdProjectItem *res = getProjectsItem( item->getClass(), item->getId() );
  //If object is found then return it
  if( res ) {
    res->setEditEnable(false);
    qDebug() << "getProjectItem from project" << item << res;
    return res;
    }
  //else insert copy
  res = dynamic_cast<SdProjectItem*>( item->copy() );
  Q_ASSERT( res != nullptr );
  insertChild( res, &mUndo );
  res->setEditEnable(false);
  qDebug() << "getProjectItem copy" << item->getId() << res->getId() << item << res;
  return res;
  }



//Find object in project with id
SdProjectItem *SdProject::getProjectsItem(quint64 mask, const QString id)
  {
  //Place for result
  SdProjectItem *res = nullptr;
  //Scan all child objects and find id
  forEach( mask, [&res, id] (SdObject *obj) -> bool {
    res = dynamic_cast<SdProjectItem*>( obj );
    Q_ASSERT( res != nullptr );
    if( res->getId() == id )
      return false;
    res = nullptr;
    return true;
    });
  return res;
  }




//Return net name unused in project
QString SdProject::getUnusedNetName()
  {
  //Prepare net pattern
  QString netName(defNetNamePrefix "%1" );
  while(true) {
    //Check if name not used
    if( isNetNameUsed( netName.arg(mNetIndex) ) )
      mNetIndex++;
    else return netName.arg(mNetIndex);
    }
  }




void SdProject::setDirty()
  {
  mDirty = true;
  }





//Return true if object with this name present in project
bool SdProject::isContains(const QString name) const
  {
  for( SdObjectPtr ptr : mChildList ) {
    SdProjectItemPtr pi = dynamic_cast<SdProjectItemPtr>( ptr );
    if( pi && pi->getTitle() == name )
      return true;
    }
  return false;
  }




//Find project item by visual tree widget item
SdObjectPtr SdProject::item(QTreeWidgetItem *src) const
  {
  for( SdObjectPtr ptr : mChildList ) {
    SdProjectItemPtr pi = dynamic_cast<SdProjectItemPtr>( ptr );
    if( pi && pi->mTreeItem == src )
      return ptr;
    }
  return 0;
  }




QString SdProject::getType() const
  {
  return QStringLiteral(SD_TYPE_PROJECT);
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




void SdProject::insertChild(SdObject *child, SdUndo *undo)
  {
  SdProjectItem *item = dynamic_cast<SdProjectItem*>( child );
  if( item && !isContains(item->getTitle()) ) {
    SdContainer::insertChild( child, undo );
    mItemExtendNameMap.insert( item->getExtendTitle(), item );
    mDirty = true;
    SdPulsar::pulsar->emitInsertItem( item );
    }
  }




void SdProject::undoInsertChild(SdObject *child)
  {
  SdProjectItem *item = dynamic_cast<SdProjectItem*>( child );
  if( item && item->getParent() == this ) {
    SdPulsar::pulsar->emitRemoveItem( item );
    mItemExtendNameMap.remove( item->getExtendTitle() );
    mDirty = true;
    SdContainer::undoInsertChild( child );
    }
  }



void SdProject::redoInsertChild(SdObject *child)
  {
  SdProjectItem *item = dynamic_cast<SdProjectItem*>( child );
  if( item ) {
    SdContainer::redoInsertChild( child );
    mItemExtendNameMap.insert( item->getExtendTitle(), item );
    mDirty = true;
    SdPulsar::pulsar->emitInsertItem( item );
    }
  }




void SdProject::deleteChild(SdObject *child, SdUndo *undo)
  {
  SdProjectItem *item = dynamic_cast<SdProjectItem*>( child );
  if( item && item->getParent() == this ) {
    SdPulsar::pulsar->emitRemoveItem( item );
    mItemExtendNameMap.remove( item->getExtendTitle() );
    mDirty = true;
    SdContainer::deleteChild( child, undo );
    }
  }




void SdProject::undoDeleteChild(SdObject *child)
  {
  SdProjectItem *item = dynamic_cast<SdProjectItem*>( child );
  if( item && item->getParent() == this ) {
    SdContainer::undoDeleteChild( child );
    mItemExtendNameMap.insert( item->getExtendTitle(), item );
    mDirty = true;
    SdPulsar::pulsar->emitInsertItem( item );
    }
  }




void SdProject::fillMap()
  {
  mItemExtendNameMap.clear();
  forEach( dctProjectItems, [this](SdObject *obj) -> bool {
    SdProjectItem *it = dynamic_cast<SdProjectItem*>(obj);
    if( it ) mItemExtendNameMap.insert( it->getExtendTitle(), it );
    return true;
    } );
  }




bool SdProject::isNetNameUsed(const QString netName)
  {
  bool unused = true;
  forEach( dctSheet, [&unused,netName] (SdObject *obj) ->bool {
    SdPItemSheet *sheet = dynamic_cast<SdPItemSheet*>(obj);
    Q_ASSERT(sheet != nullptr);
    unused = sheet->netGet( netName ) == nullptr;
    return unused;
    });
  return !unused;
  }
