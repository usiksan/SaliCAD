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


void SdObjectFactory::openLibrary()
  {
  static QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  //Test if previous dbase is opened and close it if nessesery
  if( db.isOpen() )
    db.close();

  //Test if database exist
  bool presence = QFile::exists( sdEnvir->mLibraryPath + SD_DATABASE_FILE );
  //Create directory if nessesery
  QDir().mkpath( sdEnvir->mLibraryPath );
  db.setDatabaseName( sdEnvir->mLibraryPath + SD_DATABASE_FILE );
  if( !db.open() ) {
    QMessageBox::critical(0, QObject::tr("Cannot open database"),
        QObject::tr("Unable to establish a database connection.\n"
                    "This example needs SQLite support. Please read "
                    "the Qt SQL driver documentation for information how "
                    "to build it.\n\n"
                    "Click Cancel to exit."), QMessageBox::Cancel);
    return;
    }

  if( !presence ) {
    QSqlQuery query;
    //Create objects table
    query.exec("CREATE TABLE objects (hash TEXT PRIMARY KEY, "
               " name TEXT, author TEXT, tag TEXT, timeCreate INTEGER, class INTEGER, object BLOB)");

    //Create hierarchical table
    query.exec("CREATE TABLE hierarchy (section TEXT PRIMARY KEY, path TEXT, parent TEXT, time INTEGER)");
    //Hierarchy table translation on all languages
    //translate - national language text (result of translation)
    //lang - national language name
    //section - section name (what translate)
    query.exec("CREATE TABLE translation (translate TEXT PRIMARY KEY, lang TEXT, section TEXT, time INTEGER)");
    }
  }




//Insert object to database. If in database already present newest object,
//then return its id. Older object is never inserted.
QString SdObjectFactory::insertObject(const SdProjectItem *item, QJsonObject obj)
  {
  static QHash<QString,QString> mCashe;
  QString id = item->getId();
  //At first check in the cashe
  if( mCashe.contains(id) ) {
    //Already in base. Nothing done.
    qDebug() << "cashe present";
    return mCashe.value(id);
    }
  //Add to cashe. Later object will be added to database or already in database
  //
  QSqlQuery q;
  q.prepare( QString("SELECT * FROM objects WHERE hash='%1'").arg( id ) );
  q.exec();
  if( q.first() ) {
    //Object is already in database
    if( !q.record().value( QStringLiteral("object") ).toByteArray().isEmpty() ) {
      //Add to cashe
      mCashe.insert( id, QString() );
      return QString();
      }
    }

  q.prepare( QString("SELECT * FROM objects WHERE name='%1' AND author='%2'").arg( item->getTitle() ).arg( item->getAuthor()) );
  q.exec();
  qDebug() << "name and author" << q.lastError();
  if( q.first() ) {
    //Object alredy present. Check if it is older or newer.
    if( q.value( QString("timeCreate") ).toInt() > item->getTime() ) {
      //In database newest object. Report to replace.
      mCashe.insert( id, q.value( QString("hash") ).toString() );
      return q.value( QString("hash") ).toString();
      }
    //In database older object. Replace in database

    //Delete older object.
    q.exec( QString("DELETE FROM objects WHERE hash='%1'").arg( q.value( QString("hash")).toString() ) );
    }

  //Insert new object
  qDebug() << "insert";
  q.prepare( QString("INSERT INTO objects (hash, name, author, tag, timeCreate, class, object) "
                       "VALUES (:hash, :name, :author, :tag, :timeCreate, :class, :object)") );
  q.bindValue( QStringLiteral(":hash"), id );
  q.bindValue( QStringLiteral(":name"), item->getTitle() );
  q.bindValue( QStringLiteral(":author"), item->getAuthor() );
  q.bindValue( QStringLiteral(":tag"), item->getTag() );
  q.bindValue( QStringLiteral(":timeCreate"), item->getTime() );
  q.bindValue( QStringLiteral(":class"), item->getClass() );
  q.bindValue( QStringLiteral(":object"), QVariant( QJsonDocument(obj).toBinaryData() ), QSql::Binary | QSql::In );
  q.exec();
  qDebug() << q.lastError();
  //Add to cashe
  mCashe.insert( id, QString() );
  return QString();
  }




//Extract object from database.
//If no object in local database then loading from internet
SdObject *SdObjectFactory::extractObject(const QString id, bool soft, QWidget *parent )
  {
  SdObjectMap map;
  QSqlQuery q;
  q.prepare( QString("SELECT object FROM objects WHERE hash='%1'").arg( id ) );
  q.exec();
  if( q.first() ) {
    //Object present. Load from obj
    QJsonObject jsonObj = QJsonDocument::fromBinaryData( q.value( QStringLiteral("object") ).toByteArray() ).object();
    if( jsonObj.isEmpty() ) {
      //Soft extract object from database.
      //If no object in local database then doing nothing
      if( soft ) return nullptr;

      //load object from server
      if( !SdDNetClient::getObject( parent, id ) ) {
        QMessageBox::warning( parent, QObject::tr("Error"), QObject::tr("Id '%1' can't be received from remote database").arg(id) );
        return nullptr;
        }

      //Repeate extract object
      q.prepare( QString("SELECT object FROM objects WHERE hash='%1'").arg( id ) );
      q.exec();
      if( !q.first() ) {
        //Something wrong, but object is not found
        QMessageBox::warning( parent, QObject::tr("Fatal"), QObject::tr("Id '%1' not found").arg(id) );
        return nullptr;
        }

      //Object present. Load from obj
      jsonObj = QJsonDocument::fromBinaryData( q.value( QStringLiteral("object") ).toByteArray() ).object();
      if( jsonObj.isEmpty() ) {
        //Something wrong, but object is not found
        QMessageBox::warning( parent, QObject::tr("Fatal"), QObject::tr("Id '%1' reported as loaded but it's wrong").arg(id) );
        return nullptr;
        }

      }
    //Build object
    return SdObject::read( &map, jsonObj );
    }
  QMessageBox::warning( parent, QObject::tr("Error"), QObject::tr("Id '%1' not found in database").arg(id) );
  return 0;
  }




SdObject *SdObjectFactory::extractObject(const QString name, const QString author, bool soft, QWidget *parent)
  {
  QSqlQuery q;
  q.exec( QString("SELECT hash FROM objects WHERE name='%1' AND author='%2' ORDER BY timeCreate DESC").arg( name ).arg( author ) );
  if( q.first() )
    //Object present.
    return extractObject( q.value( QStringLiteral("hash")).toString(), soft, parent );
  return nullptr;
  }





//Return true if object already present in dataBase
bool SdObjectFactory::isObjectPresent(const QString name, const QString author)
  {
  QSqlQuery q;
  q.exec( QString("SELECT hash FROM objects WHERE name='%1' AND author='%2'").arg( name ).arg( author ) );
  if( q.first() )
    //Object present.
    return true;
  return false;
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







