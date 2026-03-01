/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Class SdProject - project (file). Unit of design.
*/

#include "SdProject.h"
#include "SdProjectItem.h"
#include "SdPulsar.h"
#include "SdPItemPlate.h"
#include "SdPItemSheet.h"
#include "SdGraphNet.h"
#include "SdEnvir.h"
#include "SdCopyMapProject.h"
#include "library/SdLibraryStorage.h"

#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QByteArray>
#include <QDebug>

QSet<SdProjectPtr> sdProjectList;


SdProject::SdProject() :
  SdContainerFile(),
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





//!
//! \brief getItem   Find project item by name
//! \param itemClass Item class to find
//! \param itemName  Item name to find. Used exact name
//! \return          Found item or nullptr if not found
//!
SdProjectItem *SdProject::getItem(quint64 itemClass, const QString itemName) const
  {
  for( SdObject *obj : std::as_const(mChildList) ) {
    if( !obj->isDeleted() && (obj->getClass() & itemClass) ) {
      SdPtr<SdProjectItem> item(obj);
      if( item.isValid() && item->getTitle() == itemName )
        //Item found. Return it
        return item.ptr();
      }
    }
  //Item not found
  return nullptr;
  }








//!
//! \brief getSheet  Find sheet by name
//! \param sheetName Sheet name to find
//! \return          Sheet ptr or nullptr if not found
//!
SdPItemSheet *SdProject::getSheet(const QString sheetName) const
  {
  return dynamic_cast<SdPItemSheet*>( getItem(dctSheet, sheetName ) );
  }




//!
//! \brief getPlate  Find plate by name
//! \param plateName Plate name to find
//! \return          Plate ptr or nullptr if not found
//!
SdPItemPlate *SdProject::getPlate(const QString plateName) const
  {
  return dynamic_cast<SdPItemPlate*>( getItem( dctPlate, plateName ) );
  }




void SdProject::getHeader(SdLibraryHeader &hdr) const
  {
  SdContainerFile::getHeader( hdr );
//  hdr.mParamTable.clear();
//  hdr.mVariantFieldCount = 1;
//  hdr.mVariantTable.append( stdParamValue );

//  //TODO Fill header with sheet variants
//  forEachConst( dctSheet, [&hdr] (SdObject *obj) -> bool {
//    SdPtrConst<SdPItemSheet> sheet(obj);
//    if( sheet.isValid() )
//      hdr.mVariantTable.append( sheet->getTitle() );
//    return false;
//    } );

  }




//Return default plate and if none - create new one
SdPItemPlate *SdProject::getDefaultPlate()
  {
  //At first try to find plate in child list
  for( SdObject *obj : std::as_const(mChildList) ) {
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
SdProjectItem *SdProject::getFixedProjectItem( const SdProjectItem *item)
  {
  //If source is null then return null also
  if( item == nullptr ) return nullptr;

  //First try find equal object
  SdClass mask = item->getClass();
  SdUid   uid  = item->hashUidName();
  qint32  time = item->getTime();
  //Place for result
  SdProjectItem *res = nullptr;
  //Scan all child objects and find id
  forEach( mask, [&res, uid, time] (SdObject *obj) -> bool {
    res = dynamic_cast<SdProjectItem*>( obj );
    Q_ASSERT( res != nullptr );
    if( res->hashUidName() == uid && res->getTime() >= time )
      return false;
    res = nullptr;
    return true;
    });

  //If object is found then return it
  if( res ) {
    res->setEditEnable( false, QString() );
    //qDebug() << "getProjectItem from project" << item << res;
    return res;
    }
  //else insert copy
  SdCopyMapProject copyMap(this);
  res = dynamic_cast<SdProjectItem*>( item->copy( copyMap, false ) );
  Q_ASSERT( res != nullptr );
  insertChild( res, &mUndo );
  res->setEditEnable( false, QString() );
  //qDebug() << "getProjectItem copy" << item->getUid() << res->getUid() << item << res;
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
  if( !mDirty ) {
    mDirty = true;
    SdPulsar::sdPulsar->emitProjectStatusChanged( this );
    //qDebug() << "set dirty";
    }
  }





//!
//! \brief isNameUsed Return true if object with this name present in project
//! \param name       Name for testing
//! \param mask       Object class mask where will be tested
//! \return           true if name used for given class
//!
bool SdProject::isNameUsed(const QString name, SdClass mask ) const
  {
  bool found = false;
  //Find object with desired name in this project
  forEachConst( mask, [name,&found] (SdObject *obj) -> bool {
    SdPtr<SdProjectItem> pi(obj);
    if( pi.isValid() )
      found = pi->getTitle() == name && pi->getAuthorKey() == SdProjectItem::getDefaultAuthor();
    return !found;
    });
  return found;
  }




//!
//! \brief item Find project item by visual tree widget item
//! \param src  Tree item object for item
//! \return     Item whose src is tree object
//!
SdObjectPtr SdProject::item(QTreeWidgetItem *src) const
  {
  for( SdObjectPtr ptr : mChildList ) {
    SdPtr<SdProjectItem> pi(ptr);
    if( pi && pi->mTreeItem == src )
      return ptr;
    }
  return nullptr;
  }




//!
//! \brief item3d Find project item by visual tree widget item for 3d view
//! \param src    Tree item object for item
//! \return       Item whose src is tree object
//!
SdObjectPtr SdProject::item3d(QTreeWidgetItem *src) const
  {
  for( SdObjectPtr ptr : mChildList ) {
    SdPtr<SdProjectItem> pi(ptr);
    if( pi && pi->m3dTreeItem == src )
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





SdObjectPtr SdProject::itemByHashUidName(const QString &theHashUidName) const
  {
  SdObjectPtr ptr = nullptr;
  forEachConst( dctAll, [theHashUidName,&ptr] (SdObject *obj ) -> bool {
    SdPtr<SdProjectItem> item(obj);
    if( item && item->hashUidName() == theHashUidName ) {
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
      sheet->forEach( dctNetWire | dctNetName | dctNetParam, [&set] (SdObject *obj1) -> bool {
        SdPtr<SdGraphNet> net(obj1);
        if( net.isValid() )
          set.insert( net->getNetName() );
        return true;
        });
    return true;
    });
  return set.values();
  }





//Check newer object and mark for upgrading
void SdProject::newerCheckAndMark()
  {
  //Scan all objects and find newer in library
  //if it there then mark object with newer uid
  forEach( dctAll, [] (SdObject *obj) -> bool {
    SdPtr<SdProjectItem> item(obj);
    if( item.isValid() && item->mThereNewer != SdLibraryStorage::instance()->isLibraryObjectPresentAndNewer(item.ptr()) ) {
      item->mThereNewer = SdLibraryStorage::instance()->isLibraryObjectPresentAndNewer(item.ptr());
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








//!
//! \brief load  Load project from file
//! \param fname File name of loaded project
//! \return      Loaded project
//!
SdProject *SdProject::load(const QString fname)
  {
  //Load project
  auto prj = sdObjectOnly<SdProject>( fileJsonLoad(fname) );

  //If project loaded successfull then update library by its content
  if( prj != nullptr )
    prj->libraryUpdate();

  //Return loaded project
  return prj;
  }




//!
//! \brief save  Save project to file
//! \param fname File name for project save to
//! \return      true if saving successfull
//!
bool SdProject::save(const QString fname)
  {
  //Save project to file
  if( fileJsonSave(fname) ) {
    //Update library for project content
    libraryUpdate();
    //Clear dirty flag
    mDirty = false;
    //Notify project status changed
    SdPulsar::sdPulsar->emitProjectStatusChanged( this );
    return true;
    }
  return false;
  }




//!
//! \brief libraryUpdate Update project in library and all its item
//!
void SdProject::libraryUpdate() const
  {
  //Append all non-editable items
  auto lib = SdLibraryStorage::instance();
  forEachConst( dctComponent | dctPart | dctSymbol | dctRich, [lib] ( SdObject *obj ) ->bool {
    //Get container file object
    SdPtrConst<SdContainerFile> cf(obj);
    //Check if it valid and non-editable
    if( cf.isValid() && !cf->isEditEnable() )
      //Insert container file into library
      lib->cfObjectInsert( cf.ptr() );
    return true;
    } );

  if( !isEditEnable() )
    lib->cfObjectInsert( this );
  }



//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdProject::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdContainer::cloneFrom( src, copyMap, next );
  setDirty();
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
    setDirty();
    SdPulsar::sdPulsar->emitInsertItem( item.ptr() );
    }
  }




void SdProject::undoInsertChild(SdObject *child)
  {
  SdPtr<SdProjectItem> item(child);
  if( item && item->getParent() == this ) {
    SdPulsar::sdPulsar->emitRemoveItem( item.ptr() );
    setDirty();
    SdContainer::undoInsertChild( child );
    }
  }



void SdProject::redoInsertChild(SdObject *child)
  {
  SdPtr<SdProjectItem> item(child);
  if( item ) {
    SdContainer::redoInsertChild( child );
    setDirty();
    SdPulsar::sdPulsar->emitInsertItem( item.ptr() );
    }
  }




void SdProject::deleteChild(SdObject *child, SdUndo *undo)
  {
  SdPtr<SdProjectItem> item(child);
  if( item && item->getParent() == this ) {
    SdPulsar::sdPulsar->emitRemoveItem( item.ptr() );
    setDirty();
    SdContainer::deleteChild( child, undo );
    }
  }




void SdProject::undoDeleteChild(SdObject *child)
  {
  SdPtr<SdProjectItem> item(child);
  if( item && item->getParent() == this ) {
    SdContainer::undoDeleteChild( child );
    setDirty();
    SdPulsar::sdPulsar->emitInsertItem( item.ptr() );
    }
  }



void SdProject::redoDeleteChild(SdObject *child)
  {
  SdPtr<SdProjectItem> item(child);
  if( item && item->getParent() == this ) {
    SdPulsar::sdPulsar->emitRemoveItem( item.ptr() );
    setDirty();
    SdContainer::redoDeleteChild( child );
    }
  }






//Upgrade newer objects
bool SdProject::upgradeNewerItems(SdUndo *undo, QWidget *parent)
  {
  if( undo )
    undo->begin( QObject::tr("Upgrading newer objects"), nullptr, false );
  //Upgrage in schematic
  if( !upgradeClassProjectItem( dctSheet, undo, parent ) ) return false;
  //Upgrade in pcb
  if( !upgradeClassProjectItem( dctPlate, undo, parent ) ) return false;
  //Upgrade components, parts and symbols
  //At first, we accumulate items to be upgraded
  QList<SdProjectItemPtr> upgradeList;
  forEach( dctComponent | dctSymbol | dctPart, [&upgradeList,this] (SdObject *obj) -> bool {
    SdPtr<SdProjectItem> item(obj);
    if( item.isValid() && SdLibraryStorage::instance()->isLibraryObjectPresentAndNewer(item.ptr()) && !isUsed(obj) ) {
      //Object may be upgraded and it is not used
      upgradeList.append( item.ptr() );
      }
    return true;
    });

  //At second, we get upgraded item, remove old item and insert new one
  for( SdProjectItemPtr item : upgradeList ) {
    //Get uid of item to extract newer object
    QString uid = item->hashUidName();
    //Extract newer object
    SdProjectItem *newItem = sdObjectOnly<SdProjectItem>( SdLibraryStorage::instance()->cfObjectGet( uid ) );
    if( newItem == nullptr )
      return false;
    //Delete old item
    item->deleteObject( undo );
    //Insert new item
    getFixedProjectItem( newItem );
    //Delete intermediate object
    delete newItem;
    }
  return true;
  }




void SdProject::projectListNewerCheckAndMark()
  {
  for( auto iter = sdProjectList.cbegin(); iter != sdProjectList.cend(); iter++ )
    (*iter)->newerCheckAndMark();
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
