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
    static void         openLibrary();

    //Close library and save unsaved data
    static void         closeLibrary();

    //Insert object to database with externally created header
    static void         insertObject(const SdLibraryHeader &hdr, QJsonObject json );

    //Insert item object to database. If in database already present newest object,
    //then nothing done. Older object is never inserted.
    static void         insertItemObject( const SdProjectItem *item, QJsonObject obj );

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

    //Load object from remote server
    static bool         loadObject( const QString hash, const QString title, QWidget *parent );

    //Extract object header
    //If no object in library return false
    static bool         extractHeader( const QString id, SdLibraryHeader &hdr );

    //Function must return false to continue iteration
    //When function return true - iteration break and return true as indicator
    static bool         forEachHeader(std::function<bool(SdLibraryHeader&)> fun1 );


    //Hierarchy table
    //Append category to hierarchy
    static void         hierarchyAddItem( const QString parent, const QString item );

    //Test if category present
    static bool         hierarchyIsPresent( const QString item );

    //Fix category translation to default language
    static void         hierarchyTranslate( const QString item, const QString translate );

    //Create full path to item category
    static QString      hierarchyGetPath( const QString item );

    //Get category translation if there or category itself if none
    static QString      hierarchyGetTranslated( const QString item );

    //Build visual hierarchy tree
    static QTreeList    hierarchyGet( const QString parent );
  };

#endif // SDOBJECTFACTORY_H
