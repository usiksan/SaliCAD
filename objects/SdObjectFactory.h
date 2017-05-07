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

    //Extract object from database
    static SdObject    *buildObject(const QString id , QWidget *parent);

    //Extract object json from database
    static QJsonObject  extractObject( const QString id, QWidget *parent );
  };

#endif // SDOBJECTFACTORY_H
