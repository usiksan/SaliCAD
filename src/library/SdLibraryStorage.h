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
#include "SvLib/SvSingleton.h"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QReadWriteLock>
#include <QByteArray>
#include <QFile>
#include <QMap>
#include <functional>

class SdContainerFile;

typedef QMap<QString,SdLibraryReference> SdLibraryReferenceMap;


class SdLibraryStorage : public QObject
  {
    Q_OBJECT

    QString                mPath;
    qint32                 mCreationIndex;
    SdLibraryReferenceMap  mReferenceMap;
    QFile                  mHeaderFile;
    QFile                  mObjectFile;
    bool                   mDirty;

    mutable QReadWriteLock mLock;

    SdLibraryStorage();
  public:

    friend SdLibraryStorage *svInstance<SdLibraryStorage>();

    static SdLibraryStorage *instance() { return svInstance<SdLibraryStorage>(); }

    //!
    //! \brief objectCount Return count of referenced objects in library
    //! \return            Count of referenced objects in library
    //!
    qint32      objectCount() const { return mReferenceMap.count(); }

    //!
    //! \brief creationIndex Return current value of creation index
    //! \return              Current value of creation index
    //!
    qint32      creationIndex() const { return mCreationIndex; }

    //==================================================================
    //Common library properties

    void        init();

    //With settings library path Storage creates files if yet not created, open files and load map
    void        setLibraryPath( const QString path );

    //Flush reference map if needed
    void        flush();

    void        flushAndDelete();

    //==================================================================
    // SdContainerFile

    //Insert item object to database. If in database already present newest object,
    //then nothing done. Older object is never inserted.
    void             cfObjectInsert( const SdContainerFile *item );

    //Mark item object as deleted
    void             cfObjectDelete( const SdContainerFile *item );

    SdContainerFile *cfObjectGet( const QString uid ) const;

    //!
    //! \brief cfObjectUploaded Mark object as already uploaded. Marked object no need to be upload
    //! \param uid
    //!
    void             cfObjectUploaded( const QString uid );

    //!
    //! \brief cfIsOlder Test if object which represents by uid and time present in library and older than there is in library
    //! \param uid       uid of tested object
    //! \param time      time of locking of tested object
    //! \return          true if tested object present in library and it older then in library
    //!
    bool             cfIsOlder( const QString uid, qint32 time ) const;

    //!
    //! \brief cfIsOlder Overloaded function. Test if object present in library and older than there is in library
    //! \param item      Tested object
    //! \return          true if tested object present in library and it older then in library
    //!
    bool             cfIsOlder( const SdContainerFile *item ) const;


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

    //!
    //! \brief getUidByIndex Get object id with index which has not been downloaded
    //! \param index         Index of needed object
    //! \return              UID of object with index
    //!
    QString     getUidByIndex(qint32 index );

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

    //Get object
    // uid - object unical identificator
    QByteArray  object( const QString uid );

    //!
    //! \brief insert     Insert new object with creation reference and append header and object
    //! \param hdr        Header of inserted object
    //! \param obj        Inserted object
    //! \param downloaded true if object downloaded from remote repo
    //!
    void        insert( const SdLibraryHeader &hdr, QByteArray obj, bool downloaded );
  };


#endif // SDLIBRARYSTORAGE_H
