/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  ObjectFactory - local database to library storage
  Local database resides in sdLibraryStorage.
*/

#include "SdObjectFactory.h"
#include "SdEnvir.h"
#include "SdProjectItem.h"
#include "SdUtil.h"
#include "library/SdLibraryStorage.h"
#include "windows/SdDNetClient.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QHash>
#include <QDebug>
#include <QMessageBox>
#include <QJsonDocument>
#include <QSettings>

SdLibraryStorage sdLibraryStorage;

//Open or create library
void SdObjectFactory::openLibrary()
  {
  sdLibraryStorage.setLibraryPath( sdEnvir->mLibraryPath );
  }




//Close library and save unsaved data
void SdObjectFactory::closeLibrary()
  {
  sdLibraryStorage.flush();
  }




void SdObjectFactory::insertObject(const SdObject *obj, const SdLibraryHeader &hdr, QJsonObject json)
  {
  if( obj == nullptr )
    return;

  QString id = obj->getId();
  //If object in library then nothing done
  if( sdLibraryStorage.isObjectContains(id) )
    return;

  //Insert object
  sdLibraryStorage.insert( id, hdr, QJsonDocument(json).toBinaryData() );
  }




//Insert object to database. If in database already present newest object,
//then return its id. Older object is never inserted.
void SdObjectFactory::insertItemObject(const SdProjectItem *item, QJsonObject obj)
  {
  if( item == nullptr )
    return;

  QString id = item->getId();
  //If object in library then nothing done
  if( sdLibraryStorage.isObjectContains(id) )
    return;
  //Insert object
  SdLibraryHeader hdr;
  item->getHeader( hdr );

  sdLibraryStorage.insert( id, hdr, qCompress( QJsonDocument(obj).toBinaryData(), -1 ) );
  }




//Extract object from database.
//If no object in local database then loading from internet
SdObject *SdObjectFactory::extractObject(const QString id, bool soft, QWidget *parent )
  {
  if( id.isEmpty() )
    return nullptr;

  SdObjectMap map;

  if( !sdLibraryStorage.isObjectContains(id) ) {
    //Soft extract object from database.
    //If no object in local database then doing nothing
    if( soft ) return nullptr;

    //load object from server
    if( !SdDNetClient::getObject( parent, id ) ) {
      QMessageBox::warning( parent, QObject::tr("Error"), QObject::tr("Id '%1' can't be received from remote database").arg(id) );
      return nullptr;
      }
    }

  //At this point object already in local base
  if( sdLibraryStorage.isObjectContains(id) )
    //Build object
    return SdObject::read( &map, QJsonDocument::fromBinaryData( qUncompress(sdLibraryStorage.object(id)) ).object() );

  QMessageBox::warning( parent, QObject::tr("Error"), QObject::tr("Id '%1' not found in database").arg(id) );
  return nullptr;
  }








//Return true if object already present in dataBase
bool SdObjectFactory::isObjectPresent(const QString name, const QString author)
  {
  return sdLibraryStorage.forEachHeader( [name,author] (SdLibraryHeader &hdr) -> bool {
    if( hdr.mName == name && hdr.mAuthor == author )
      return true;
    return false;
    });
  }




//Return true if object present in dataBase
bool SdObjectFactory::isObjectPresent(const QString hash)
  {
  return sdLibraryStorage.isObjectContains(hash);
  }





//Load object from remote server
bool SdObjectFactory::loadObject(const QString hash, const QString title, QWidget *parent)
  {
  if( hash.isEmpty() )
    return false;

  if( sdLibraryStorage.isObjectContains(hash) )
    return true;

  //load object from server
  if( !SdDNetClient::getObject( parent, hash ) ) {
    QMessageBox::warning( parent, QObject::tr("Error"), QObject::tr("Obj '%1' can't be received from remote database").arg(title) );
    return false;
    }

  return true;
  }





//Extract object header
//If no object in library return false
bool SdObjectFactory::extractHeader(const QString id, SdLibraryHeader &hdr)
  {
  return sdLibraryStorage.header( id, hdr );
  }





bool SdObjectFactory::forEachHeader(std::function<bool (SdLibraryHeader &)> fun1)
  {
  return sdLibraryStorage.forEachHeader( fun1 );
  }






void SdObjectFactory::hierarchyAddItem(const QString parent, const QString item)
  {
  if( hierarchyIsPresent(item) )
    //Ignore request. Nothing done
    return;

  //Insert new item
  sdLibraryStorage.categoryInsert( item, parent, false );
  }



//Return true if item present in hierarchy table
bool SdObjectFactory::hierarchyIsPresent(const QString item)
  {
  return sdLibraryStorage.isCategoryContains( item );
  }




void SdObjectFactory::hierarchyTranslate(const QString item, const QString translate)
  {
  //Language of user
  QSettings s;
  QString lang = s.value( QStringLiteral(SDK_LANGUAGE) ).toString();

  if( lang.isEmpty() )
    return;

  lang.append( "+_" ).append( item );
  sdLibraryStorage.categoryInsert( lang, translate, false );
  }




QString SdObjectFactory::hierarchyGetPath(const QString item)
  {
  QString path(item);
  for( QString parent(item); !parent.isEmpty() && parent != sdLibraryStorage.category(parent); parent = sdLibraryStorage.category(parent) ) {
    path = parent + QString(".") + path;
    }
  return path;
  }




//Hierarchy table translation
QString SdObjectFactory::hierarchyGetTranslated(const QString item)
  {
  //Language of user
  QSettings s;
  QString lang = s.value( QStringLiteral(SDK_LANGUAGE) ).toString();

  if( lang.isEmpty() )
    return item;

  lang.append( "+_" ).append( item );
  if( sdLibraryStorage.isCategoryContains(lang) )
    return sdLibraryStorage.category( lang );

  return item;
  }






//Hierarchy table
QTreeList SdObjectFactory::hierarchyGet(const QString parent)
  {
  QTreeList list;
  sdLibraryStorage.forEachCategory( [&list, parent] (const QString section, const QString &assoc ) {
    if( assoc == parent ) {
      //Translate section to user language
      QString text = hierarchyGetTranslated( section );
      //Create new item
      QTreeWidgetItem *item = new QTreeWidgetItem();
      item->setText( 0, text );
      item->setText( 1, section );

      QTreeList childList = hierarchyGet( section );
      if( childList.empty() )
        //Item is leaf
        item->setIcon( 0, QIcon(QString(":/pic/brFile.png")) );
      else {
        //Item is branch
        item->setIcon( 0, QIcon(QString(":/pic/brDir.png")) );
        item->addChildren( childList );
        }

      //Append item to list
      list.append( item );
      }
    });

  return list;
  }







