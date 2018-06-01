/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  ObjectFactory - local database to library storage
  Local database resides in SQLite file.
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

static SdLibraryStorage library;

//Open or create library
void SdObjectFactory::openLibrary()
  {
  library.setLibraryPath( sdEnvir->mLibraryPath );
  }




//Close library and save unsaved data
void SdObjectFactory::closeLibrary()
  {
  library.flush();
  }




//Insert object to database. If in database already present newest object,
//then return its id. Older object is never inserted.
void SdObjectFactory::insertObject(const SdProjectItem *item, QJsonObject obj)
  {
  if( item == nullptr )
    return;

  QString id = item->getId();
  //If object in library then nothing done
  if( library.isObjectContains(id) )
    return;
  //Insert object
  SdLibraryHeader hdr;
  item->getHeader( hdr );

  library.insert( id, hdr, QJsonDocument(obj).toBinaryData() );
  }




//Extract object from database.
//If no object in local database then loading from internet
SdObject *SdObjectFactory::extractObject(const QString id, bool soft, QWidget *parent )
  {
  if( id.isEmpty() )
    return nullptr;

  SdObjectMap map;

  if( !library.isObjectContains(id) ) {
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
  if( library.isObjectContains(id) )
    //Build object
    return SdObject::read( &map, QJsonDocument::fromBinaryData( library.object(id) ).object() );

  QMessageBox::warning( parent, QObject::tr("Error"), QObject::tr("Id '%1' not found in database").arg(id) );
  return nullptr;
  }




//SdObject *SdObjectFactory::extractObject(const QString name, const QString author, bool soft, QWidget *parent)
//  {
//  QSqlQuery q;
//  q.exec( QString("SELECT hash FROM objects WHERE name='%1' AND author='%2' ORDER BY timeCreate DESC").arg( name ).arg( author ) );
//  if( q.first() )
//    //Object present.
//    return extractObject( q.value( QStringLiteral("hash")).toString(), soft, parent );
//  return nullptr;
//  }





//Return true if object already present in dataBase
bool SdObjectFactory::isObjectPresent(const QString name, const QString author)
  {
  return library.forEachHeader( [name,author] (SdLibraryHeader &hdr) -> bool {
    if( hdr.mName == name && hdr.mAuthor == author )
      return true;
    return false;
    });
  }





//Extract object header
//If no object in library return false
bool SdObjectFactory::extractHeader(const QString id, SdLibraryHeader &hdr)
  {
  return library.header( id, hdr );
  }





bool SdObjectFactory::forEachHeader(std::function<bool (SdLibraryHeader &)> fun1)
  {
  return library.forEachHeader( fun1 );
  }






void SdObjectFactory::hierarchyAddItem(const QString parent, const QString item)
  {
  QSqlQuery q;
  if( hierarchyIsPresent(item) )
    //Ignore request. Nothing done
    return;
    //q.exec( QString("DELETE FROM hierarchy WHERE section='%1'").arg( item ) );

  //Insert new item
  QString path = hierarchyGetPath( parent );
  qDebug() << "insert hierarchy item" << parent << item;
  q.prepare( QString("INSERT INTO hierarchy (section, path, parent, time) "
                     "VALUES (:section, :path, :parent, :time)") );
  q.bindValue( QStringLiteral(":section"), item );
  q.bindValue( QStringLiteral(":path"), path + QString(".") + item );
  q.bindValue( QStringLiteral(":parent"), parent );
  q.bindValue( QStringLiteral(":time"), SdUtil::getTime2000() );
  q.exec();
  }



//Return true if item present in hierarchy table
bool SdObjectFactory::hierarchyIsPresent(const QString item)
  {
  QSqlQuery q;
  q.exec( QString("SELECT section FROM hierarchy WHERE section='%1'").arg( item ) );
  return q.first();
  }




void SdObjectFactory::hierarchyTranslate(const QString item, const QString translate)
  {
  //Language of user
  QSettings s;
  QString lang = s.value( QStringLiteral(SDK_LANGUAGE) ).toString();

  if( lang.isEmpty() )
    return;

  QSqlQuery q;
  if( hierarchyGetTranslated(item) != item )
    //Item already precent in table, delete it
    q.exec( QString("DELETE FROM translation WHERE section='%1' AND lang='%2'").arg( item ).arg(lang) );

  //Insert new translate record
  qDebug() << "insert translate item" << item << translate;
  q.prepare( QString("INSERT INTO translation (section, lang, translate, time) "
                     "VALUES (:section, :lang, :translate, :time)") );
  q.bindValue( QStringLiteral(":section"), item );
  q.bindValue( QStringLiteral(":lang"), lang );
  q.bindValue( QStringLiteral(":translate"), translate );
  q.bindValue( QStringLiteral(":time"), SdUtil::getTime2000() );
  q.exec();
  }



QString SdObjectFactory::hierarchyGetPath(const QString item)
  {
  QSqlQuery q;
  q.exec( QString("SELECT path FROM hierarchy WHERE section='%1'").arg( item ) );
  if( q.first() )
    //Element present, return its path
    return q.value( QStringLiteral("path") ).toString();
  return item;
  }




//Hierarchy table translation
QString SdObjectFactory::hierarchyGetTranslated(const QString item)
  {
  //Language of user
  QSettings s;
  QString lang = s.value( QStringLiteral(SDK_LANGUAGE) ).toString();

  if( lang.isEmpty() )
    return item;


  QSqlQuery q;
  q.exec( QString("SELECT translate FROM translation WHERE section='%1' AND lang='%2'").arg( item ).arg(lang) );
  if( q.first() )
    //Translation present, return it
    return q.value( QStringLiteral("translate") ).toString();

  return item;
  }






//Hierarchy table
QTreeList SdObjectFactory::hierarchyGet(const QString parent)
  {
  QTreeList list;
  QSqlQuery q;
  q.exec( QString("SELECT section FROM hierarchy WHERE parent='%1'").arg( parent ) );
  if( q.first() ) {
    do {
      //For each finded elements we creating QTreeWidgetItem and append it to list
      QString section = q.value( QStringLiteral("section") ).toString();
      //Translate section to user language
      QString text = hierarchyGetTranslated( section );
      //Create item for this section
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
    //Take next record
    while( q.next() );
    }

  return list;
  }







