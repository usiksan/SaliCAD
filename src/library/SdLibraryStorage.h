﻿/*
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
#include <QSet>
#include <QTimer>
#include <QFileInfoList>
#include <functional>

class SdContainerFile;
class SdNetClientLocker;

typedef QMap<QString,SdLibraryReference> SdLibraryReferenceMap;


class SdLibraryStorage : public QObject
  {
    Q_OBJECT

    QString                mLibraryPath;     //!< Path where library objects resides
    QFileInfoList          mScanList;        //!< Library scan list contains list of subdirectories of library
    QTimer                 mScanTimer;       //!< Scan timer for periodic scan library for new or deleted objects
    QMap<QString,QString>  mExistList;       //!< Map of existing objects
    SdLibraryReferenceMap  mReferenceMap;
    QFile                  mHeaderFile;
    SdNetClientLocker     *mNetClientLocker;
    bool                   mDirty;
    bool                   mUploadAvailable; //!< If true then there available objects to upload
    bool                   mNewestMark;      //!< True if need to update newst marks

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


    //==================================================================
    // Library common
    //!
    //! \brief libraryPath Return current library path
    //! \return            Current library path
    //!
    QString     libraryPath() const { return mLibraryPath; }

    //!
    //! \brief libraryPathSet Set new library path
    //! \param path           New library path
    //!
    //! Current caches are cleared, library path changed and scan start from begin
    void        libraryPathSet( const QString &path );

    //!
    //! \brief libraryInit Init library system
    //!
    void        libraryInit();

    //!
    //! \brief libraryComplete Complete work of libraryStorage. We flush caches and delete libraryStorage
    //!
    void             libraryComplete();


    //==================================================================
    // SdContainerFile

    //!
    //! \brief cfObjectInsert Insert object into library. If in library already present newest object
    //!                       then nothing done. Older object is never inserted.
    //! \param item           Object for inserting
    //!
    void             cfObjectInsert( const SdContainerFile *item );

    //Mark item object as deleted
    void             cfObjectDelete( const SdContainerFile *item );

    //!
    //! \brief cfObjectGet Load object from library
    //! \param uid         UID of loaded object
    //! \return            Loaded object or nullptr
    //!
    SdContainerFile *cfObjectGet( const QString uid ) const;

    //!
    //! \brief cfObjectUpload Return object available to uploading to remote server
    //! \return               Object to uploading
    //!
    SdContainerFile *cfObjectUpload() const;

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

    //!
    //! \brief cfIsOlderOrSame Test if object which represents by uid and time present in library and older or same than there is in library
    //! \param uid             uid of tested object
    //! \param time            time of locking of tested object
    //! \return                true if tested object present in library and it older then in library
    //!
    bool             cfIsOlderOrSame( const QString uid, qint32 time ) const;

    //!
    //! \brief cfIsOlderOrSame Overloaded function. Test if object present in library and older or same than there is in library
    //! \param item            Tested object
    //! \return                true if tested object present in library and it older then in library
    //!
    bool             cfIsOlderOrSame( const SdContainerFile *item ) const;

    //!
    //! \brief cfObjectContains Return true if object contains in library
    //! \param uid              UID of object
    //! \return                 True if object contains in library
    //!
    bool             cfObjectContains( const QString uid ) const { return mReferenceMap.contains(uid) && !mReferenceMap.value(uid).isNeedDelete(); }


    //==================================================================
    //Headers

    //!
    //! \brief header Get header of object
    //! \param uid    object unical identificator
    //! \param hdr    place to receiv object header
    //! \return       true if header readed successfully
    //!
    bool             header( const QString uid, SdLibraryHeader &hdr );


    //!
    //! \brief forEachHeader Scan all available headers and call functor fun1 for each of them
    //! \param fun1          Functor which called for each header
    //!                      Function must return false to continue iteration
    //!                      When function return true - iteration break and return true as indicator
    //! \return              true if at least one functor return true
    //!
    bool             forEachHeader( std::function<bool(SdLibraryHeader&)> fun1 );




  signals:

    //Append information item
    void             informationAppended( const QString info );

    void             updateNewestMark();

  private slots:
    //!
    //! \brief periodicScan Perform scan step
    //!
    void             periodicScan();

  private:
    //!
    //! \brief insertReferenceAndHeader Insert in cache reference to object and header of object
    //! \param item                     Object which inserted
    //!
    void             insertReferenceAndHeader( const SdContainerFile *item );

    //!
    //! \brief fullPath Return full path to library file
    //! \param fileName Library file fileName
    //! \return         Full path to library file
    //!
    QString          fullPath( const QString &fileName ) const;

    static QString   cachePath();
  };


#endif // SDLIBRARYSTORAGE_H
