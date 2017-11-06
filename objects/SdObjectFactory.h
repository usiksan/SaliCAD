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
#include <QJsonObject>
#include <QWidget>

class SdProjectItem;

class SdObjectFactory
  {
  public:
    //Open or create library
    static void openLibrary();

    //Insert object to database. If in database already present newest object,
    //then return its id. Older object is never inserted.
    static QString      insertObject( const SdProjectItem *item, QJsonObject obj );

    //Extract object from database.
    //If no object in local database then loading from internet
    //Soft extract object from database.
    //If no object in local database then doing nothing
    static SdObject    *extractObject( const QString id, bool soft, QWidget *parent );
    static SdObject    *extractObject( const QString name, const QString author, bool soft, QWidget *parent );

    //Return true if object already present in dataBase
    static bool         isObjectPresent( const QString name, const QString author );
  };

#endif // SDOBJECTFACTORY_H
