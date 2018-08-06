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
#include "SdObjectFactory.h"
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QByteArray>
#include <QDebug>

QSet<SdProjectPtr> sdProjectList;


SdProject::SdProject() :
  mDirty(false),
  mNetIndex(0)
  {
  //Add project to global project list
  sdProjectList.insert( this );
  }




SdProject::~SdProject()
  {
  sdProjectList.remove( this );
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
  plate->setTitle( QStringLiteral("Default plate"), QString() );
  insertChild( plate, &mUndo );
  return plate;
  }




//Return object of project for given object
SdProjectItem *SdProject::getFixedProjectItem(SdProjectItem *item)
  {
  //If source is null then return null also
  if( item == nullptr ) return item;
  //First try find equal object
  SdClass mask = item->getClass();
  SdUid   uid  = item->getUid();
  qint32  time = item->getTime();
  //Place for result
  SdProjectItem *res = nullptr;
  //Scan all child objects and find id
  forEach( mask, [&res, uid, time] (SdObject *obj) -> bool {
    res = dynamic_cast<SdProjectItem*>( obj );
    Q_ASSERT( res != nullptr );
    if( res->getUid() == uid && res->getTime() >= time )
      return false;
    res = nullptr;
    return true;
    });

  //If object is found then return it
  if( res ) {
    res->setEditEnable( false, QString() );
    qDebug() << "getProjectItem from project" << item << res;
    return res;
    }
  //else insert copy
  res = dynamic_cast<SdProjectItem*>( item->copy() );
  Q_ASSERT( res != nullptr );
  insertChild( res, &mUndo );
  res->setEditEnable( false, QString() );
  qDebug() << "getProjectItem copy" << item->getUid() << res->getUid() << item << res;
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






//Accum used layers
void SdProject::accumLayerUsage()
  {
  forEach( dctAll, [] ( SdObject *obj ) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph != nullptr )
      graph->setLayerUsage();
    return true;
    } );
  }




void SdProject::setDirty()
  {
  mDirty = true;
  }





//Return true if object with this name present in project
bool SdProject::isNameUsed(const QString name) const
  {
  //Find object with desired name in this project
  for( SdObjectPtr ptr : mChildList ) {
    SdProjectItemPtr pi = dynamic_cast<SdProjectItemPtr>( ptr );
    if( pi && !pi->isDeleted() && pi->getTitle() == name && pi->getAuthor() == SdProjectItem::getDefaultAuthor() )
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
  return nullptr;
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
      return sdObjectOnly<SdProject>( SdObject::read( &map, doc.object() ) );
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
  if( item ) {
    SdContainer::insertChild( child, undo );
    mItemExtendNameMap.insert( item->getExtendTitle(), item );
    mDirty = true;
    SdPulsar::sdPulsar->emitInsertItem( item );
    }
  }




void SdProject::undoInsertChild(SdObject *child)
  {
  SdProjectItem *item = dynamic_cast<SdProjectItem*>( child );
  if( item && item->getParent() == this ) {
    SdPulsar::sdPulsar->emitRemoveItem( item );
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
    SdPulsar::sdPulsar->emitInsertItem( item );
    }
  }




void SdProject::deleteChild(SdObject *child, SdUndo *undo)
  {
  SdProjectItem *item = dynamic_cast<SdProjectItem*>( child );
  if( item && item->getParent() == this ) {
    SdPulsar::sdPulsar->emitRemoveItem( item );
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
    SdPulsar::sdPulsar->emitInsertItem( item );
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
    unused = !sheet->isNetPresent( netName );
    return unused;
    });
  return !unused;
  }
