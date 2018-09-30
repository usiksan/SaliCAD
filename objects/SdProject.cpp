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
#include "SdGraphNet.h"
#include "SdEnvir.h"

#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QByteArray>
#include <QDebug>

QSet<SdProjectPtr> sdProjectList;


SdProject::SdProject() :
  SdContainer(),
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





//Return first sheet
SdPItemSheet *SdProject::getFirstSheet() const
  {
  for( SdObject *obj : mChildList ) {
    if( !obj->isDeleted() && obj->getClass() == dctSheet ) {
      //Sheet found. Return it
      return dynamic_cast<SdPItemSheet*>(obj);
      }
    }
  //Sheet not found
  return nullptr;
  }




//Return first plate
SdPItemPlate *SdProject::getFirstPlate() const
  {
  for( SdObject *obj : mChildList ) {
    if( !obj->isDeleted() && obj->getClass() == dctPlate ) {
      //Plate found. Return it
      return dynamic_cast<SdPItemPlate*>(obj);
      }
    }
  //Plate not found
  return nullptr;
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
    SdPtr<SdProjectItem> pi(obj);
    if( pi )
      pi->setLayerUsage();
    return true;
    } );
  }




//Renumeration implements
void SdProject::renumeration()
  {
  //For each plate perform renumeration
  forEach( dctPlate, [] (SdObject *obj) -> bool {
    SdPtr<SdPItemPlate> plate(obj);
    if( plate )
      plate->renumeration();
    return true;
    });
  }




//Sheet auto renumeration
void SdProject::sheetRenumeration()
  {
  //For each sheet assign continuously number
  int index = 1;
  forEach( dctSheet, [this,&index] (SdObject *obj) -> bool {
    SdPtr<SdPItemSheet> sheet(obj);
    if( sheet )
      sheet->setSheetIndex( index++, &mUndo );
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
    SdPtr<SdProjectItem> pi(ptr);
    if( pi && !pi->isDeleted() && pi->getTitle() == name && pi->getAuthor() == SdProjectItem::getDefaultAuthor() )
      return true;
    }
  return false;
  }




//Find project item by visual tree widget item
SdObjectPtr SdProject::item(QTreeWidgetItem *src) const
  {
  for( SdObjectPtr ptr : mChildList ) {
    SdPtr<SdProjectItem> pi(ptr);
    if( pi && pi->mTreeItem == src )
      return ptr;
    }
  return nullptr;
  }





SdObjectPtr SdProject::itemByName(quint64 mask, const QString name) const
  {
  SdObjectPtr ptr = nullptr;
  forEachConst( mask, [name,&ptr] (SdObject *obj ) -> bool {
    SdPtr<SdProjectItem> item(obj);
    if( item && item->getTitle() == name ) {
      ptr = item.ptr();
      return false;
      }
    return true;
    });
  return ptr;
  }





SdObjectPtr SdProject::itemByUid(const QString &uid) const
  {
  SdObjectPtr ptr = nullptr;
  forEachConst( dctAll, [uid,&ptr] (SdObject *obj ) -> bool {
    SdPtr<SdProjectItem> item(obj);
    if( item && item->getUid() == uid ) {
      ptr = item.ptr();
      return false;
      }
    return true;
    });
  return ptr;
  }





//Return full net list from all project sheets
QStringList SdProject::netList() const
  {
  //For all sheets accum net list
  QSet<QString> set;
  //For each sheet accum net list
  forEachConst( dctSheet, [&set] (SdObject *obj) -> bool {
    SdPtr<SdPItemSheet> sheet(obj);
    if( sheet.isValid() )
      //In sheet for each net object append unical net name to list
      sheet->forEach( dctNetWire | dctNetName, [&set] (SdObject *obj1) -> bool {
        SdPtr<SdGraphNet> net(obj1);
        if( net.isValid() )
          set.insert( net->getNetName() );
        return true;
        });
    return true;
    });
  return set.toList();
  }





//Check newer object and mark for upgrading
void SdProject::newerCheckAndMark()
  {
  //Scan all objects and find newer in library
  //if it there then mark object with newer uid
  forEach( dctAll, [] (SdObject *obj) -> bool {
    SdPtr<SdProjectItem> item(obj);
    if( item.isValid() && item->mThereNewer != SdObjectFactory::isThereNewer(item.ptr()) ) {
      item->mThereNewer = SdObjectFactory::isThereNewer(item.ptr());
      SdPulsar::sdPulsar->emitRenameItem( item.ptr() );
      }
    return true;
    });
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
  }




void SdProject::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdContainer::readObject( map, obj );
  }




SdProject *SdProject::load(const QString fname)
  {
  QFile file(fname);
  if( file.open(QIODevice::ReadOnly) ) {
    QJsonDocument doc = QJsonDocument::fromJson( file.readAll(), nullptr );
    if( !doc.isEmpty() ) {
      SdObjectMap map;
      return sdObjectOnly<SdProject>( SdObject::read( &map, doc.object() ) );
      }
    }
  return nullptr;
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
  //const SdProject *sour = dynamic_cast<const SdProject*>(src);
  }




void SdProject::insertChild(SdObject *child, SdUndo *undo)
  {

  if( child != nullptr && child->getClass() == dctSheet ) {
    //When sheet inserting we assign it next sheet index
    //At first count existing sheet number
    int sheetCount = 1;
    forEach( dctSheet, [&sheetCount] (SdObject *obj) -> bool {
      Q_UNUSED(obj)
      sheetCount++;
      return true;
      });
    //Assign next number to be inserted sheet
    SdPtr<SdPItemSheet> sheet(child);
    if( sheet )
      sheet->setSheetIndex( sheetCount, undo );
    }

  //Common insertion
  SdPtr<SdProjectItem> item(child);
  if( item ) {
    SdContainer::insertChild( child, undo );
    mDirty = true;
    SdPulsar::sdPulsar->emitInsertItem( item.ptr() );
    }
  }




void SdProject::undoInsertChild(SdObject *child)
  {
  SdPtr<SdProjectItem> item(child);
  if( item && item->getParent() == this ) {
    SdPulsar::sdPulsar->emitRemoveItem( item.ptr() );
    mDirty = true;
    SdContainer::undoInsertChild( child );
    }
  }



void SdProject::redoInsertChild(SdObject *child)
  {
  SdPtr<SdProjectItem> item(child);
  if( item ) {
    SdContainer::redoInsertChild( child );
    mDirty = true;
    SdPulsar::sdPulsar->emitInsertItem( item.ptr() );
    }
  }




void SdProject::deleteChild(SdObject *child, SdUndo *undo)
  {
  SdPtr<SdProjectItem> item(child);
  if( item && item->getParent() == this ) {
    SdPulsar::sdPulsar->emitRemoveItem( item.ptr() );
    mDirty = true;
    SdContainer::deleteChild( child, undo );
    }
  }




void SdProject::undoDeleteChild(SdObject *child)
  {
  SdPtr<SdProjectItem> item(child);
  if( item && item->getParent() == this ) {
    SdContainer::undoDeleteChild( child );
    mDirty = true;
    SdPulsar::sdPulsar->emitInsertItem( item.ptr() );
    }
  }



void SdProject::redoDeleteChild(SdObject *child)
  {
  SdPtr<SdProjectItem> item(child);
  if( item && item->getParent() == this ) {
    SdPulsar::sdPulsar->emitRemoveItem( item.ptr() );
    mDirty = true;
    SdContainer::redoDeleteChild( child );
    }
  }








//Test if net name already used
bool SdProject::isNetNameUsed(const QString netName)
  {
  bool unused = true;
  forEach( dctSheet, [&unused,netName] (SdObject *obj) ->bool {
    SdPtr<SdPItemSheet> sheet(obj);
    Q_ASSERT( sheet.isValid() );
    unused = !sheet->isNetPresent( netName );
    return unused;
    });
  return !unused;
  }
