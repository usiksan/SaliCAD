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
               " name TEXT, author TEXT, time INTEGER, rank INTEGER, object BLOB)");
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
  q.prepare( QString("INSERT INTO objects (hash, status, name, author, time, rank, object) "
                       "VALUES (:hash, :status, :name, :author, :time, :rank, :object)") );
  q.bindValue( QString(":hash"), id );
  q.bindValue( QString(":status"), 0 );
  q.bindValue( QString(":name"), item->getTitle() );
  q.bindValue( QString(":author"), item->getAuthor() );
  q.bindValue( QString(":time"), item->getTime() );
  q.bindValue( QString(":rank"), 0 );
  q.bindValue( QString(":object"), QVariant(obj) );
  q.exec();
  qDebug() << q.lastError();
  //Add to cashe
  mCashe.insert( id, QString() );
  return QString();
  }
