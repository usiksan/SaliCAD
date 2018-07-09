/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  All variable object saved in library. It are cad objects, translate strings, hierarhy system and so on
  Every object have its unical id (hash). With its hash we store reference to header - small object portion and
  to object - main object body.

  All information saved in three files: references file, headers file and objects file. References file contains
  map of all known objects with reference to its header and object.

  Storage complains all three files and formalized access to it.
*/
#ifndef SDLIBRARYSTORAGE_H
#define SDLIBRARYSTORAGE_H

#include "SdLibraryReference.h"
#include "SdLibraryHeader.h"
#include "SdLibraryCategory.h"

#include <QString>
#include <QStringList>
#include <QReadWriteLock>
#include <QByteArray>
#include <QFile>
#include <QMap>
#include <functional>

typedef QMap<QString,SdLibraryReference> SdLibraryReferenceMap;

typedef QMap<QString,SdLibraryCategory> SdLibraryCategoryMap;

class SdLibraryStorage
  {
  protected:
    QString                mPath;
    QReadWriteLock         mLock;
    qint32                 mCreationIndex;
    SdLibraryReferenceMap  mReferenceMap;
    SdLibraryCategoryMap   mCategoryMap;
    QFile                  mHeaderFile;
    QFile                  mObjectFile;
    bool                   mDirty;
  public:
    SdLibraryStorage();
    ~SdLibraryStorage();

    //With settings library path Storage creates files if yet not created, open files and load map
    void        setLibraryPath( const QString path );

    //Return true if object referenced in map
    bool        contains( const QString key );

    //Return true if object contained in map
    bool        isObjectContains( const QString key );

    //Get list of objects which inserted after index
    QStringList getAfter( qint32 index, int limit = 100 );

    //Function must return false to continue iteration
    //When function return true - iteration break and return true as indicator
    bool        forEachHeader(std::function<bool(SdLibraryHeader&)> fun1 );

    //Get header of object
    bool        header( const QString key, SdLibraryHeader &hdr );

    //Set reference to object with header
    void        setHeader( const QString key, SdLibraryHeader &hdr );

    //Get object
    QByteArray  object( const QString key );

    //Set object to reference. Reference must be exist
    void        setObject( const QString key, QByteArray obj );

    //Insert new object with creation reference and append header and object
    void        insert(const QString key, const SdLibraryHeader &hdr, QByteArray obj );

    //Return true if key contains in categories list
    bool        isCategoryContains( const QString key );

    //Return category association map
    QString     category( const QString key );

    //Insert new category
    void        categoryInsert( const QString key, const QString association );

    //Get list of categories which inserted after index
    QStringList categoryGetAfter(qint32 index, int limit = 100 );

    //Flush reference map if needed
    void        flush();
  };


extern SdLibraryStorage sdLibraryStorage;

#endif // SDLIBRARYSTORAGE_H
