/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdObjectFactory.h"
#include "SdEnvir.h"
#include "SdProjectItem.h"
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
    query.exec("CREATE TABLE objects (hash TEXT PRIMARY KEY, status INTEGER,"
               " name TEXT, author TEXT, time INTEGER, class INTEGER, rank INTEGER, object BLOB)");
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
  q.prepare( QString("SELECT hash FROM objects WHERE hash='%1'").arg( id ) );
  q.exec();
  if( q.first() ) {
    //Object is already in database
    //Add to cashe
    mCashe.insert( id, QString() );
    return QString();
    }

  q.prepare( QString("SELECT * FROM objects WHERE name='%1' AND author='%2'").arg( item->getTitle() ).arg( item->getAuthor()) );
  q.exec();
  qDebug() << "name and author" << q.lastError();
  if( q.first() ) {
    //Object alredy present. Check if it is older or newer.
    if( q.value( QString("time") ).toInt() > item->getTime() ) {
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
  q.prepare( QString("INSERT INTO objects (hash, status, name, author, time, class, rank, object) "
                       "VALUES (:hash, :status, :name, :author, :time, :class, :rank, :object)") );
  q.bindValue( QStringLiteral(":hash"), id );
  q.bindValue( QStringLiteral(":status"), 0 );
  q.bindValue( QStringLiteral(":name"), item->getTitle() );
  q.bindValue( QStringLiteral(":author"), item->getAuthor() );
  q.bindValue( QStringLiteral(":time"), item->getTime() );
  q.bindValue( QStringLiteral(":class"), item->getClass() );
  q.bindValue( QStringLiteral(":rank"), 0 );
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

      //TODO D025 load object from server
      }
    //Build object
    SdObjectMap map;
    return SdObject::read( &map, jsonObj );
    }
  QMessageBox::warning( parent, QObject::tr("Error"), QObject::tr("Id '%1' not found in database").arg(id) );
  return 0;
  }




SdObject *SdObjectFactory::extractObject(const QString name, const QString author, bool soft, QWidget *parent)
  {
  QSqlQuery q;
  q.exec( QString("SELECT hash FROM objects WHERE name='%1' AND author='%2'").arg( name ).arg( author ) );
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







