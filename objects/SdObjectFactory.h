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

class SdProjectItem;

typedef QList<QTreeWidgetItem*> QTreeList;

class SdObjectFactory
  {
  public:
    //Open or create library
    static void openLibrary();

    //Close library and save unsaved data
    static void closeLibrary();

    //Insert object to database. If in database already present newest object,
    //then return its id. Older object is never inserted.
    static void         insertObject( const SdProjectItem *item, QJsonObject obj );

    //Extract object from database.
    //If no object in local database then loading from internet
    //Soft extract object from database.
    //If no object in local database then doing nothing
    static SdObject    *extractObject( const QString id, bool soft, QWidget *parent );

    //Return true if object already present in dataBase
    static bool         isObjectPresent( const QString name, const QString author );

    //Extract object header
    //If no object in library return false
    static bool         extractHeader( const QString id, SdLibraryHeader &hdr );

    //Function must return false to continue iteration
    //When function return true - iteration break and return true as indicator
    static bool         forEachHeader(std::function<bool(SdLibraryHeader&)> fun1 );


    //Hierarchy table
    static void         hierarchyAddItem( const QString parent, const QString item );
    static bool         hierarchyIsPresent( const QString item );
    static void         hierarchyTranslate( const QString item, const QString translate );
    static QString      hierarchyGetPath( const QString item );
    static QString      hierarchyGetTranslated( const QString item );
    static QTreeList    hierarchyGet( const QString parent );
  };

#endif // SDOBJECTFACTORY_H
