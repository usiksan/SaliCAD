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

    qint32      objectCount() const { return mReferenceMap.count(); }

    //==================================================================
    //Common library properties

    //With settings library path Storage creates files if yet not created, open files and load map
    void        setLibraryPath( const QString path );

    //Flush reference map if needed
    void        flush();



    //==================================================================
    //Objects

    //Return true if object referenced in map
    bool        contains( const QString uid );

    //Return true if object contained in map
    bool        isObjectContains( const QString uid );

    //Return true if object referenced in map and object is newer or same or if newer reference
    bool        isNewerOrSameObject( const QString uid, qint32 time );

    //Return true if newer object referenced in map
    bool        isNewerObject( const QString uid, qint32 time );

    //Get list of objects which inserted after index
    QStringList getAfter( qint32 index, int limit = 100 );

    //Function must return false to continue iteration
    //When function return true - iteration break and return true as indicator
    bool        forEachHeader(std::function<bool(SdLibraryHeader&)> fun1 );

    //Function for iteration on all or partial uid's
    //When function return true - iteration break, else countinued
    void        forEachUid( std::function<bool(const QString &uid)> fun1 );

    //Get header of object
    // uid - object unical identificator
    // hdr - place to receiv object header
    bool        header( const QString uid, SdLibraryHeader &hdr );

    //Set reference to object with header
    // hdr - object header
    bool        setHeader( SdLibraryHeader &hdr );

    //Get object
    // uid - object unical identificator
    QByteArray  object( const QString uid );

    //Insert new object with creation reference and append header and object
    void        insert( const SdLibraryHeader &hdr, QByteArray obj );
  };


extern SdLibraryStorage sdLibraryStorage;

#endif // SDLIBRARYSTORAGE_H
