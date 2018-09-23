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

#include <QString>
#include <QStringList>
#include <QReadWriteLock>
#include <QByteArray>
#include <QFile>
#include <QMap>
#include <functional>

typedef QMap<QString,SdLibraryReference> SdLibraryReferenceMap;


class SdLibraryStorage
  {
  protected:
    QString                mPath;
    QReadWriteLock         mLock;
    qint32                 mCreationIndex;
    SdLibraryReferenceMap  mReferenceMap;
    QFile                  mHeaderFile;
    QFile                  mObjectFile;
    bool                   mDirty;
  public:
    SdLibraryStorage();
    ~SdLibraryStorage();

    //==================================================================
    //Common library properties

    //With settings library path Storage creates files if yet not created, open files and load map
    void        setLibraryPath( const QString path );

    //Flush reference map if needed
    void        flush();



    //==================================================================
    //Objects

    //Return true if object referenced in map
    bool        contains( const QString key );

    //Return true if object contained in map
    bool        isObjectContains( const QString key );

    //Return true if newer object referenced in map
    bool        isNewerObject( const QString key, qint32 time );

    //Get list of objects which inserted after index
    QStringList getAfter( qint32 index, int limit = 100 );

    //Function must return false to continue iteration
    //When function return true - iteration break and return true as indicator
    bool        forEachHeader(std::function<bool(SdLibraryHeader&)> fun1 );

    //Get header of object
    bool        header( const QString key, SdLibraryHeader &hdr );

    //Set reference to object with header
    // key - object key
    // hdr - object header
    // remote - if true then object referenced as received from remote
    //          with mCreationIndex=-1, else referenced as next created
    void        setHeader( SdLibraryHeader &hdr, bool remote = false );

    //Get object
    QByteArray  object( const QString key );

    //Insert new object with creation reference and append header and object
    void        insert( const SdLibraryHeader &hdr, QByteArray obj );
  };


extern SdLibraryStorage sdLibraryStorage;

#endif // SDLIBRARYSTORAGE_H
