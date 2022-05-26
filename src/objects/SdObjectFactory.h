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

#ifndef SDOBJECTFACTORY_H
#define SDOBJECTFACTORY_H

#include "SdObject.h"
#include "library/SdLibraryHeader.h"

#include <QJsonObject>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QList>
#include <QSet>

class SdContainerFile;

typedef QList<QTreeWidgetItem*> QTreeList;

typedef QSet<QString> QStringSet;

class SdObjectFactory
  {
  public:
    //Open or create library
    static void         openLibrary();

    //Close library and save unsaved data
    static void         closeLibrary();

    //Insert object to database with externally created header
    static void         insertObject(const SdLibraryHeader &hdr, QJsonObject json );

    //Insert item object to database. If in database already present newest object,
    //then nothing done. Older object is never inserted.
    static void         insertItemObject( const SdContainerFile *item, QJsonObject obj );

    //Mark item object as deleted
    static void         deleteItemObject( const SdContainerFile *item );

    //Extract object from database.
    //If no object in local database then loading from internet
    //Soft extract object from database.
    //If no object in local database then doing nothing
    static SdObject    *extractObject( const QString id, bool soft, QWidget *parent );

    //Return true if object already present in dataBase
    static bool         isObjectPresent( const QString name, const QString author );

    //Return true if object present in dataBase
    static bool         isObjectPresent( const QString hash );

    //Return true if object name is referenced in dataBase
    static bool         isContains( const QString type, const QString name, const QString author );

    //Return true if there is newer object in dataBase
    static bool         isThereNewer( const SdContainerFile *item );

    //Load object from remote server
    static bool         loadObject( const QString hash, const QString title, QWidget *parent );

    //Extract object header
    //If no object in library return false
    static bool         extractHeader( const QString id, SdLibraryHeader &hdr );

    //Function for iteration on all or partial uid's
    //When function return true - iteration break, else continued
    static void         forEachUid( std::function<bool(const QString &uid)> fun1 );

    //Function for iteration on all or partial headers
    //When function return true - iteration break, else continued
    static void         forEachHeader( std::function<bool( SdLibraryHeader &hdr )> fun1 );

  };

#endif // SDOBJECTFACTORY_H
