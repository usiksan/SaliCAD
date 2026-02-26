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
#include "SdAuthorDescription.h"
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
//class QCborMap;

typedef QMap<QString,SdLibraryReference> SdLibraryReferenceMap;

typedef QMap<QString,SdAuthorDescription> SdAuthorAssocMap;


class SdLibraryStorage : public QObject
  {
    Q_OBJECT

    QString                mLibraryPath;       //!< Path where library objects resides
    QFileInfoList          mScanList;          //!< Library scan list contains list of subdirectories of library
    QTimer                 mScanTimer;         //!< Scan timer for periodic scan library for new or deleted objects
    QMap<QString,QString>  mExistList;         //!< Map of existing objects
    SdLibraryReferenceMap  mReferenceMap;      //!< Map with key of object id and value is SdLibraryReference
    QFile                  mHeaderFile;        //!< File with header of all objects in library
    SdAuthorAssocMap       mAuthorAssoc;       //!< Author public hash to author name association
    qint32                 mPrivateLastSync;   //!< Time of last sync with private cloud
    qint32                 mPrivateLastList;   //!< Time of last list from private cloud
    qint32                 mGlobalLastSync;    //!< Time of last sync with global storage
    qint32                 mGlobalLastList;    //!< Time of last list from global storage
    bool                   mDirty;
    bool                   mNewestMark;        //!< True if need to update newst marks

    mutable QReadWriteLock mLock;

    //Statistic
    int                    mLocalAppended;
    int                    mLocalRemoved;
    int                    mLocalUpdated;
    bool                   mLocalTransfer;

    int                    mGlobalTransferIn;
    int                    mGlobalTransferOut;
    int                    mIterGlobalTransferIn;
    int                    mIterGlobalTransferOut;
    QString                mGlobalError;

    int                    mCloudTransferIn;
    int                    mCloudTransferOut;
    int                    mIterCloudTransferIn;
    int                    mIterCloudTransferOut;
    QString                mCloudError;

    bool                   mPause;

    SdLibraryStorage();
  public:
    SV_SINGLETON( SdLibraryStorage )


    //!
    //! \brief objectCount Return count of referenced objects in library
    //! \return            Count of referenced objects in library
    //!
    qint32           objectCount() const { return mReferenceMap.count(); }


    //==================================================================
    // Library common
    //!
    //! \brief libraryPeriodicBreakResume Breaks or resume scan and sync process
    //! \param breakOn         When true scan and sync broken
    //!
    void             libraryPeriodicBreakResume( bool breakOn );

    //!
    //! \brief libraryPath Return current library path
    //! \return            Current library path
    //!
    QString          libraryPath() const { return mLibraryPath; }

    //!
    //! \brief libraryPathSet Set new library path
    //! \param path           New library path
    //!
    //! Current caches are cleared, library path changed and scan start from begin
    void             libraryPathSet( const QString &path );

    //!
    //! \brief libraryInit Init library system
    //!
    void             libraryInit();

    //!
    //! \brief libraryComplete Complete work of libraryStorage. We flush caches and delete libraryStorage
    //!
    void             libraryComplete();

    //!
    //! \brief globalStorageSyncReset Reset sync process with global storage. Sync begins from clear.
    //!
    void             globalStorageSyncReset();

    //!
    //! \brief privateCloudSyncReset Reset sync process with private cloud. Sync begins from clear.
    //!
    void             privateCloudSyncReset();

    //!
    //! \brief privateCloudNameNew Generate new private cloud name
    //! \return                    Private cloud name
    //!
    static QString   privateCloudNameNew() ;

    //!
    //! \brief privateCloudName Returns current private cloud namr
    //! \return                 Current private name
    //!
    static QString   privateCloudName();



    //==================================================================
    // SdContainerFile

    //!
    //! \brief cfObjectInsert Insert object into library. If in library already present newest object
    //!                       then nothing done. Older object is never inserted.
    //! \param item           Object for inserting
    //!
    void               cfObjectInsert( const SdContainerFile *item );

    //Mark item object as deleted
    void               cfObjectDelete( const SdContainerFile *item );

    //!
    //! \brief cfObjectGet Load object from library
    //! \param uid         UID of loaded object
    //! \return            Loaded object or nullptr
    //!
    SdContainerFile   *cfObjectGet( const QString hashUidName ) const;


    //!
    //! \brief cfIsOlder   Test if object which represents by hashUidName and time present in library and older than there is in library
    //! \param hashUidName hashUidName of tested object
    //! \param time        time of locking of tested object
    //! \return            true if tested object present in library and it older then in library
    //!
    bool               cfIsOlder( const QString hashUidName, qint32 time ) const;

    //!
    //! \brief cfIsOlder Overloaded function. Test if object present in library and older than there is in library
    //! \param item      Tested object
    //! \return          true if tested object present in library and it older then in library
    //!
    bool               cfIsOlder( const SdContainerFile *item ) const;

    //!
    //! \brief cfIsOlderOrSame Test if object which represents by uid and time present in library and older or same than there is in library
    //! \param hashUidName     hashUidName of tested object
    //! \param time            time of locking of tested object
    //! \return                true if tested object present in library and it older then in library
    //!
    bool               cfIsOlderOrSame( const QString hashUidName, qint32 time ) const;

    //!
    //! \brief cfIsOlderOrSame Overloaded function. Test if object present in library and older or same than there is in library
    //! \param item            Tested object
    //! \return                true if tested object present in library and it older then in library
    //!
    bool               cfIsOlderOrSame( const SdContainerFile *item ) const;

    //!
    //! \brief cfObjectContains Return true if object contains in library
    //! \param hashUidName      hashUidName of object
    //! \return                 True if object contains in library
    //!
    bool               cfObjectContains( const QString hashUidName ) const;


    SdLibraryReference cfReference( const QString &hashUidName ) const;

    bool               cfIsPresentAndPrivate( const QString &hashUidName ) const;
    //==================================================================
    //Headers

    //!
    //! \brief header      Get header of object
    //! \param hashUidName object unical identificator
    //! \param hdr         place to receiv object header
    //! \return            true if header readed successfully
    //!
    bool             header( const QString hashUidName, SdLibraryHeader &hdr );


    //!
    //! \brief forEachHeader Scan all available headers and call functor fun1 for each of them
    //! \param fun1          Functor which called for each header
    //!                      Function must return false to continue iteration
    //!                      When function return true - iteration break and return true as indicator
    //! \return              true if at least one functor return true
    //!
    bool             forEachHeader( std::function<bool(SdLibraryHeader&)> fun1 );


    void             forEachReference( std::function<void( const QString &hashUidName, const SdLibraryReference &ref )> fun1 );


    //==================================================================
    // Author association
    //!
    //! \brief authorGlobalName Return author visual name by author public key
    //! \param authorPublicKey  Author public key
    //! \return                 Author visual name
    //!
    QString          authorGlobalName( const QString &authorPublicKey ) const;

    QString          authorGlobalNameWithRank( const QString &authorPublicKey ) const;

    static QString   authorPrivateKeyNew();

    static QString   authorPublicKey();

    static QString   convertSaliCadUidToHash( const QString &saliCadUid );
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
    //! \brief objectInsertPrivate Insert object into library. If in library already present newest object
    //!                            then nothing done. Older object is never inserted.
    //! \param item                Object for inserting
    //! \param downloaded          true when object downloaded from remote clouds
    //!
    void             objectInsertPrivate( const SdContainerFile *item, bool downloaded );

    //!
    //! \brief insertReferenceAndHeader Insert in cache reference to object and header of object
    //! \param item                     Object which inserted
    //! \param downloaded               true when object loaded from cloud
    //!
    void             insertReferenceAndHeader( const SdContainerFile *item, bool downloaded );

    //!
    //! \brief fullPath Return full path to library file
    //! \param item     Item, for which path builded
    //! \return         Full path to library file
    //!
    QString          fullPath( const SdContainerFile *item ) const;

    //!
    //! \brief fullPath Return full path to library file
    //! \param fileName Library file fileName
    //! \return         Full path to library file
    //!
    QString          fullPath( const QString &fileName ) const;

    //!
    //! \brief fileNameOfLibraryObject Returns file name of library object
    //! \param hashUidName             Unical id of object
    //! \return                        File name of library object
    //!
    QString          fileNameOfLibraryObject( const QString &hashUidName ) const;

    //!
    //! \brief fullPathOfLibraryObject Returns full path to object in library
    //! \param hashUidName             Unical id of object
    //! \return                        Full path to file of object in library
    //!
    QString          fullPathOfLibraryObject( const QString &hashUidName ) const;

    //!
    //! \brief libraryBuildStructure Builds structure of library
    //!
    void             libraryBuildStructure();

    QByteArray       objectGet( const QString &hashUidName ) const;

    QByteArray       objectGetFromFile( const QString &fileName ) const;

    void             objectDelete(const QString &hashUidName, qint32 timeCreation , bool isPrivate);

    void             objectPut( const QByteArray &content );

    QCborMap         prepareQuery(int queryType, const SdFileUid &uid, bool appendObject = false ) const;

    static QCborMap  prepareQueryList( int queryType, int time );

    static QString   cachePath();

    static void      remoteSync( SdLibraryStorage *storage );
  };


#endif // SDLIBRARYSTORAGE_H
