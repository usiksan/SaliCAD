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




void SdObjectFactory::insertObject( const SdLibraryHeader &hdr, QJsonObject json)
  {

  //Insert object
  sdLibraryStorage.insert( hdr, qCompress( QJsonDocument(json).toBinaryData(), -1 ), false );
  }




//Insert object to database. If in database already present newest object,
//then return its id. Older object is never inserted.
void SdObjectFactory::insertItemObject(const SdContainerFile *item, QJsonObject obj)
  {
  if( item == nullptr || sdLibraryStorage.isNewerOrSameObject( item->getUid(), item->getTime() ) )
    return;

  //Insert object
  SdLibraryHeader hdr;
  item->getHeader( hdr );

  insertObject( hdr, obj );
  }





//Mark item object as deleted
void SdObjectFactory::deleteItemObject(const SdContainerFile *item)
  {
  if( item == nullptr )
    return;
  SdLibraryHeader hdr;
  //Extract existing header and test if it corresponds to item
  if( sdLibraryStorage.header( item->getUid(), hdr ) && item->getTime() == hdr.mTime ) {
    //Mark header as deleted
    hdr.setDeleted();
    //Store "new" object
    insertObject( hdr, QJsonObject() );
    }
  }




//Extract object from database.
//If no object in local database then loading from internet
SdObject *SdObjectFactory::extractObject(const QString id, bool soft, QWidget *parent )
  {
  if( id.isEmpty() )
    return nullptr;

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
    return SdObject::jsonObjectFrom( QJsonDocument::fromBinaryData( qUncompress(sdLibraryStorage.object(id)) ).object() );

  QMessageBox::warning( parent, QObject::tr("Error"), QObject::tr("Id '%1' not found in database").arg(id) );
  return nullptr;
  }








//Return true if object present in dataBase
bool SdObjectFactory::isObjectPresent(const QString hash)
  {
  return sdLibraryStorage.isObjectContains(hash);
  }






//Return true if object name is referenced in dataBase
bool SdObjectFactory::isContains(const QString type, const QString name, const QString author)
  {
  return sdLibraryStorage.contains( headerUid( type, name, author ) );
  }





//Return true if there is newer object in dataBase
bool SdObjectFactory::isThereNewer(const SdContainerFile *item)
  {
  if( item == nullptr )
    return false;
  return sdLibraryStorage.isNewerObject( item->getUid(), item->getTime() );
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








//Function for iteration on all or partial uid's
//When function return true - iteration break, else countinued
void SdObjectFactory::forEachUid(std::function<bool (const QString &)> fun1)
  {
  sdLibraryStorage.forEachUid( fun1 );
  }




//Function for iteration on all or partial headers
//When function return true - iteration break, else continued
void SdObjectFactory::forEachHeader(std::function<bool (SdLibraryHeader &)> fun1)
  {
  sdLibraryStorage.forEachHeader( fun1 );
  }










