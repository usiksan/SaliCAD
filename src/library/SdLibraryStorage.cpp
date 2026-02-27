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
#include "SdLibraryStorage.h"
#include "SdLibraryIndicator.h"
#include "SdLibraryServerDef.h"
#include "objects/SdContainerFile.h"
#include "SdTcpCborClient.h"
#include "SvLib/SvDir.h"
#include "SvLib/SvTime2x.h"

#include <QDir>
#include <QReadLocker>
#include <QWriteLocker>
#include <QSaveFile>
#include <QFileInfo>
#include <QSettings>
#include <QDebug>
#include <QtConcurrent/QtConcurrentRun>
#include <QCborMap>
#include <QCborArray>
#include <QCoreApplication>
#include <QMetaObject>
#include <QCryptographicHash>
#include <QUuid>


#define FNAME_REF    cachePath() + QStringLiteral("reference.dat")
#define FNAME_HDR    cachePath() + QStringLiteral("headers.dat")
#define HDR_START    23

#define REFERENCE_SIGNATURE QString("SalixEDA reference file v1")

SdLibraryStorage::SdLibraryStorage() :
  QObject(),
  mReferenceMap(),
  mHeaderFile(),
  mPrivateLastSync(0),   //!< Time of last sync with private cloud
  mPrivateLastList(0),   //!< Time of last list from private cloud
  mGlobalLastSync(0),    //!< Time of last sync with global storage
  mGlobalLastList(0),    //!< Time of last list from global storage
  mAuthorLastList(0),
  mDirty(false),
  mNewestMark(false),
  mLock(),
  mLocalAppended(0),
  mLocalRemoved(0),
  mLocalUpdated(0),
  mLocalTransfer(false),
  mGlobalTransferIn(0),
  mGlobalTransferOut(0),
  mIterGlobalTransferIn(0),
  mIterGlobalTransferOut(0),
  mCloudTransferIn(0),
  mCloudTransferOut(0),
  mIterCloudTransferIn(0),
  mIterCloudTransferOut(0),
  mPause(false)
  {
  connect( &mScanTimer, &QTimer::timeout, this, &SdLibraryStorage::periodicScan );
  }




//!
//! \brief libraryPeriodicBreakResume Breaks or resume scan and sync process
//! \param breakOn                    When true scan and sync broken
//!
void SdLibraryStorage::libraryPeriodicBreakResume(bool breakOn)
  {
  if( breakOn ) {
    //Signal pause
    mPause = true;
    mScanTimer.stop();
    }
  else {
    mPause = false;
    mScanTimer.start( 1000 );
    }
  }





//!
//! \brief libraryPathSet Set new library path
//! \param path           New library path
//!
//! Current caches are cleared, library path changed and scan start from begin
void SdLibraryStorage::libraryPathSet(const QString &path)
  {
  if( path != mLibraryPath ) {
    QWriteLocker locker( &mLock );
    //Stop scan timer
    mScanTimer.stop();

    //Build new library directory
    SvDir dir( path );
    mLibraryPath = dir.slashedPath();
    QSettings s;
    s.setValue( QStringLiteral(SDK_LOCAL_LIBRARY_PATH), mLibraryPath );

    //Clear cache reference map
    mReferenceMap.clear();
    //and remove its file
    if( QFile::exists( FNAME_REF ) )
      QFile::remove( FNAME_REF );
    //Remove header file
    if( QFile::exists( FNAME_HDR ) )
      QFile::remove( FNAME_HDR );

    //Clear current scan process
    mScanList.clear();

    //Clear statistic
    mPrivateLastSync = mPrivateLastList = mGlobalLastSync = mGlobalLastList = mAuthorLastList = 0;
    mLocalAppended = mLocalRemoved = mLocalUpdated = 0;
    mLocalTransfer = false;
    mGlobalTransferIn = mGlobalTransferOut = mIterGlobalTransferIn = mIterGlobalTransferOut = 0;
    mCloudTransferIn = mCloudTransferOut = mIterCloudTransferIn = mIterCloudTransferOut = 0;

    libraryBuildStructure();
    }
  }




//!
//! \brief libraryInit Init library system
//!
void SdLibraryStorage::libraryInit()
  {
  QWriteLocker locker( &mLock );

  mLocalAppended = mLocalRemoved = mLocalUpdated = 0;
  mLocalTransfer = false;
  mGlobalTransferIn = mGlobalTransferOut = mIterGlobalTransferIn = mIterGlobalTransferOut = 0;
  mCloudTransferIn = mCloudTransferOut = mIterCloudTransferIn = mIterCloudTransferOut = 0;

  libraryBuildStructure();
  }




//!
//! \brief libraryComplete Complete work of libraryStorage. We flush caches and delete libraryStorage
//!
void SdLibraryStorage::libraryComplete()
  {
  saveDirtyCache();

  //Close previously opened data base
  if( mHeaderFile.isOpen() )
    mHeaderFile.close();

  deleteLater();
  }




//!
//! \brief globalStorageSyncReset Reset sync process with global storage. Sync begins from clear.
//!
void SdLibraryStorage::globalStorageSyncReset()
  {
  mGlobalLastSync = mGlobalLastList = mAuthorLastList = 0;
  mDirty = true;
  }




//!
//! \brief privateCloudSyncReset Reset sync process with private cloud. Sync begins from clear.
//!
void SdLibraryStorage::privateCloudSyncReset()
  {
  mPrivateLastSync = mPrivateLastList = 0;
  mDirty = true;
  }




//!
//! \brief privateCloudNameNew Generate new private cloud name
//! \return                    Private cloud name
//!
QString SdLibraryStorage::privateCloudNameNew()
  {
  return QStringLiteral("SlxCloud-") + QUuid::createUuid().toString( QUuid::WithoutBraces );
  }





//!
//! \brief privateCloudName Returns current private cloud name
//! \return                 Current private name
//!
QString SdLibraryStorage::privateCloudName()
  {
  QSettings s;
  if( !s.contains(SDK_PRIVATE_CLOUD_NAME) ) {
    s.setValue( SDK_PRIVATE_CLOUD_NAME, privateCloudNameNew() );
    }
  qDebug() << "Private cloud name" << s.value( SDK_PRIVATE_CLOUD_NAME ).toString();
  return s.value( SDK_PRIVATE_CLOUD_NAME ).toString();
  }




//!
//! \brief cfObjectInsert Insert object into library. If in library already present newest object
//!                       then nothing done. Older object is never inserted.
//! \param item           Object for inserting
//!
void SdLibraryStorage::cfObjectInsert(const SdContainerFile *item)
  {
  objectInsertPrivate( item, false );
  }




void SdLibraryStorage::cfObjectDelete(const SdContainerFile *item)
  {
  if( item != nullptr && item->getAuthorKey() == authorPublicKey() ) {
    QWriteLocker locker( &mLock );
    QString uid = item->hashUidName();
    if( mReferenceMap.contains(uid) ) {
      //Mark as need to be deleted from server
      mReferenceMap[uid].markAsRemoved( SvTime2x::current() );

      //Delete object file
      QFile::remove( fullPathOfLibraryObject( uid ) );

      mDirty = true;
      }
    }
  }





//!
//! \brief cfObjectGet Load object from library
//! \param hashUidName UID of loaded object
//! \return            Loaded object or nullptr
//!
SdContainerFile *SdLibraryStorage::cfObjectGet(const QString hashUidName) const
  {
  QReadLocker locker( &mLock );
  if( mReferenceMap.contains( hashUidName ) && !mReferenceMap[hashUidName].isRemoved() ) {
    //Load object
    return sdObjectOnly<SdContainerFile>( SdObject::fileJsonLoad( fullPathOfLibraryObject(hashUidName) ) );
    }
  return nullptr;
  }







//!
//! \brief isLibraryObjectPresentAndNewer   Test if object which represents by hashUidName and time present in library and older than there is in library
//! \param hashUidName hashUidName of tested object
//! \param time        time of locking of tested object
//! \return            true if tested object present in library and it older then in library
//!
bool SdLibraryStorage::isLibraryObjectPresentAndNewer(const QString hashUidName, qint32 time) const
  {
  QReadLocker locker( &mLock );
  return mReferenceMap.contains( hashUidName ) && mReferenceMap.value(hashUidName).isNewer( time );
  }



//!
//! \brief isLibraryObjectPresentAndNewer Overloaded function. Test if object present in library and older than there is in library
//! \param item      Tested object
//! \return          true if tested object present in library and it older then in library
//!
bool SdLibraryStorage::isLibraryObjectPresentAndNewer(const SdContainerFile *item) const
  {
  if( item == nullptr ) return false;
  return isLibraryObjectPresentAndNewer( item->hashUidName(), item->getTime() );
  }





//!
//! \brief isLibraryObjectOlderOrNone   Test if object which represent by hashUidName and time not present in library or newer than in library
//! \param hashUidName hashUidName of tested object
//! \param time        time of locking of tested object
//! \return            true if object which represent by hashUidName and time not present in library or newer than in library
//!
bool SdLibraryStorage::isLibraryObjectOlderOrNone(const QString hashUidName, qint32 time) const
  {
  QReadLocker locker( &mLock );
  //Library object is older than time, so time is newer
  return !mReferenceMap.contains( hashUidName ) || mReferenceMap.value(hashUidName).isOlder( time );
  }



//!
//! \brief isLibraryObjectOlderOrNone Overloaded function. Test if object is not present in library or item object is newer than in library
//! \param fileUid                    Tested object
//! \return                           true if object is not present in library or fileUid object is newer than in library
//!
bool SdLibraryStorage::isLibraryObjectOlderOrNone(const SdContainerFile *item) const
  {
  return isLibraryObjectOlderOrNone( item->fileUid() );
  }



bool SdLibraryStorage::cfObjectContains(const QString hashUidName) const
  {
  QReadLocker locker( &mLock );
  return mReferenceMap.contains(hashUidName) && !mReferenceMap.value(hashUidName).isRemoved();
  }






SdLibraryReference SdLibraryStorage::cfReference(const QString &hashUidName) const
  {
  QReadLocker locker( &mLock );
  return mReferenceMap.value( hashUidName );
  }








//!
//! \brief header      Get header of object
//! \param hashUidName object unical identificator
//! \param hdr         place to receiv object header
//! \return            true if header readed successfully
//!
bool SdLibraryStorage::header(const QString hashUidName, SdLibraryHeader &hdr) const
  {
  //For empty key return false to indicate no header
  if( hashUidName.isEmpty() ) return false;

  QWriteLocker locker( &mLock );
  if( !mReferenceMap.contains(hashUidName) || mReferenceMap.value(hashUidName).isRemoved() ) return false;
  mHeaderFile.seek( mReferenceMap.value(hashUidName).mHeaderPtr );
  QDataStream is( &mHeaderFile );
  hdr.read( is );
  return true;
  }






//!
//! \brief forEachHeader Scan all available headers and call functor fun1 for each of them
//! \param fun1          Functor which called for each header
//!                      Function must return false to continue iteration
//!                      When function return true - iteration break and return true as indicator
//! \return              true if at least one functor return true
//!
bool SdLibraryStorage::forEachHeader(std::function<bool(SdLibraryHeader&)> fun1)
  {
  QReadLocker locker( &mLock );
  mHeaderFile.seek( HDR_START );
  QDataStream is( &mHeaderFile );
  SdLibraryHeader hdr;
  while( !mHeaderFile.atEnd() ) {
    auto position = mHeaderFile.pos();
    hdr.read( is );
   // qDebug() << hdr.uid() << hdr.mTime << mHeaderFile.pos();
    //Test if header not deleted
    if( mReferenceMap.value(hdr.hashUidName()).mHeaderPtr == position ) {
      if( fun1( hdr) )
        return true;
      }
    }
  return false;
  }




void SdLibraryStorage::forEachReference(std::function<void (const QString &, const SdLibraryReference &)> fun1)
  {
  QReadLocker locker( &mLock );
  for( auto it = mReferenceMap.cbegin(); it != mReferenceMap.cend(); it++ )
    fun1( it.key(), it.value() );
  }



QString SdLibraryStorage::authorOfObject(const QString &hashUidName) const
  {
  SdLibraryHeader hdr;
  if( header( hashUidName, hdr ) ) {
    return hdr.mAuthorKey;
    }
  return QString{};
  }




QString SdLibraryStorage::authorGlobalName(const QString &authorPublicKey) const
  {
  QReadLocker locker( &mLock );
  if( mAuthorAssoc.contains(authorPublicKey) )
    return mAuthorAssoc.value(authorPublicKey ).authorName();
  return tr("Anonim");
  }




QString SdLibraryStorage::authorGlobalNameWithRank(const QString &authorPublicKey) const
  {
  QReadLocker locker( &mLock );
  if( mAuthorAssoc.contains(authorPublicKey) )
    return mAuthorAssoc.value(authorPublicKey ).authorNameWithRank();
  return tr("Anonim [○□]");
  }




QString SdLibraryStorage::authorPrivateKeyNew()
  {
  return QStringLiteral("SlxUser-") + QUuid::createUuid().toString( QUuid::WithoutBraces );
  }




QString SdLibraryStorage::authorPublicKey()
  {
  static QString cachePrivateKey;
  static QString cacheAuthorKey;
  QSettings s;

  if( !s.contains(SDK_PRIVATE_KEY) ) {
    //Author not assigned yet
    s.setValue( SDK_PRIVATE_KEY, authorPrivateKeyNew() );
    }

  if( s.value( SDK_PRIVATE_KEY ).toString() != cachePrivateKey ) {
    cachePrivateKey = s.value( SDK_PRIVATE_KEY ).toString();
    QByteArray hash = QCryptographicHash::hash( cachePrivateKey.toUtf8(), QCryptographicHash::Blake2s_128 );
    cacheAuthorKey = QString::fromUtf8( hash.toHex() ).first( 16 );
    }

  return cacheAuthorKey;
  }





QString SdLibraryStorage::convertSaliCadUidToHash(const QString &saliCadUid)
  {
  //Split uid to separate parts
  QStringList list = saliCadUid.split( sdUidDelimiter );
  if( list.count() != 3 )
    return QString{};
  return SdContainerFile::hashUidName( list.at(0), list.at(1), authorPublicKey() );
  }





//!
//! \brief periodicScan Perform scan step
//!
void SdLibraryStorage::periodicScan()
  {
  if( mPause ) return;
  if( mScanList.count() ) {
    //Continue scan
    //Here we scan single subdirectory
    QFileInfo subdir = mScanList.takeLast();

    //List of files for subdirectory
    QString subdr = subdir.filePath();
    if( !subdr.endsWith( QChar('/') ) )
      subdr.append( QChar('/') );

    QFileInfoList fileList = QDir(subdr).entryInfoList( QDir::Files );

    for( const auto &file : std::as_const(fileList) ) {
      QString fileName = file.fileName();

      SdFileUid fileUid( fileName );
      if( isLibraryObjectPresentAndNewer( fileUid ) ) {
        //This file older than in library and can be removed
        //Remove this file because it is older than in library
        QFile::remove( file.absoluteFilePath() );
        mLocalRemoved++;
        mLocalTransfer = true;
        }
      else if( isLibraryObjectOlderOrNone( fileUid ) ) {
        //Replace old object or append new
        //Statistic update
        if( mReferenceMap.contains( fileUid.mHashUidName ) ) {
          //Will be update because older object in library
          mLocalUpdated++;
          mNewestMark = true;
          }
        else
          //Will be simple append. No same object in library
          mLocalAppended++;
        mLocalTransfer = true;

        if( QFile::exists(file.absoluteFilePath()) ) {
          QScopedPointer<SdContainerFile> item( sdObjectOnly<SdContainerFile>(SdObject::fileJsonLoad(file.absoluteFilePath())) );
          //Fix crash: check if item readed correctly
          if( item.isNull() ) continue;
          objectInsertPrivate( item.data(), false );
          }
        }
      }

    if( mScanList.count() == 0 ) {
      mScanTimer.stop();
      //Scan is complete
      // if( removeCount )
      //   emit informationAppended( tr("Remove external deleted objects %1").arg( removeCount )  );

      //Inform about loop complete
      SdLibraryIndicator::instance()->setLocalLibrary( mReferenceMap.count(), mLocalAppended, mLocalRemoved, mLocalUpdated, mLocalTransfer ? SdLibraryIndicator::SdLisTransfer : SdLibraryIndicator::SdLisNoTransfer, QString{} );

      //Prepare remote sync
      mIterGlobalTransferIn = mIterGlobalTransferOut = 0;
      mGlobalError.clear();

      mIterCloudTransferIn = mIterCloudTransferOut = 0;
      mCloudError.clear();

      //Perform sync with remote storage
      QtConcurrent::run( SdLibraryStorage::remoteSync, this ).then( this, [this] () {
        if( mPause ) return;

        //Send statistic to UI

        //First, global storage
        if( mGlobalError.isEmpty() ) {
          //No error at global sync
          //Define is it was transfer or not
          int stat = mIterGlobalTransferIn == 0 && mIterGlobalTransferOut == 0 ? SdLibraryIndicator::SdLisNoTransfer : SdLibraryIndicator::SdLisTransfer;
          //Increment total variables (session)
          mGlobalTransferIn += mIterGlobalTransferIn;
          mGlobalTransferOut += mIterGlobalTransferOut;
          //Send signal to UI
          SdLibraryIndicator::instance()->setGlobalStorage( mGlobalTransferOut, mGlobalTransferIn, stat, QString{} );
          }
        else
          //Was error
          SdLibraryIndicator::instance()->setGlobalStorage( 0, 0, SdLibraryIndicator::SdLisError, mGlobalError );

        //Second, private cloud
        if( mCloudError.isEmpty() ) {
          //No error at global sync
          //Define is it was transfer or not
          int stat = mIterCloudTransferIn == 0 && mIterCloudTransferOut == 0 ? SdLibraryIndicator::SdLisNoTransfer : SdLibraryIndicator::SdLisTransfer;
          //Increment total variables (session)
          mCloudTransferIn += mIterCloudTransferIn;
          mCloudTransferOut += mIterCloudTransferOut;
          //Send signal to UI
          SdLibraryIndicator::instance()->setPrivateCloud( mCloudTransferOut, mCloudTransferIn, stat, QString{} );
          }
        else
          //Was error
          SdLibraryIndicator::instance()->setPrivateCloud( 0, 0, SdLibraryIndicator::SdLisError, mCloudError );

        mScanTimer.start( 30000 );
        });

      if( mNewestMark ) {
        mNewestMark = false;
        emit updateNewestMark();
        }

      }
    }
  else {

    //Begin new scan
    QDir dir( mLibraryPath );
    mScanList = dir.entryInfoList( QDir::AllDirs | QDir::NoDotAndDotDot );

    if( mScanList.count() ) {

      mLocalTransfer = false;

      mScanTimer.start( 100 );
      }
    else {
      //In library directory no subdir. It is not prepared or it not library directory
      //Report error
      SdLibraryIndicator::instance()->setLocalLibrary( mReferenceMap.count(), mLocalAppended, mLocalRemoved, mLocalUpdated, SdLibraryIndicator::SdLisError, tr("Fail scan local library directory<br>Check library path in settings") );
      mScanTimer.start( 30000 );
      }

    }
  }




void SdLibraryStorage::addObjectCompliant(const SdFileUid &fileUid, const QString &message)
  {
  QWriteLocker locker( &mLock );
  SdCompliantDescr descr;
  descr.mFileUid = fileUid;
  descr.mMessage = message;
  mCompliantDescrList.append( descr );
  mDirty = true;
  }





//!
//! \brief objectInsertPrivate Insert object into library. If in library already present newest object
//!                            then nothing done. Older object is never inserted.
//! \param item                Object for inserting
//! \param downloaded          true when object downloaded from remote clouds
//!
void SdLibraryStorage::objectInsertPrivate(const SdContainerFile *item, bool downloaded)
  {
  //If inserted object is older than present in library then nothing done
  if( mLibraryPath.isEmpty() || item == nullptr || item->isEditEnable() )
    return;

  if( isLibraryObjectOlderOrNone(item) ) {
    //Insert reference and header
    insertReferenceAndHeader( item, downloaded );

    //Insert in library
    QString filePath = fullPath( item );

    item->fileJsonSave( filePath );
    }
  }




//!
//! \brief insertReferenceAndHeader Insert in cache reference to object and header of object
//! \param item                     Object which inserted
//!
void SdLibraryStorage::insertReferenceAndHeader(const SdContainerFile *item, bool downloaded)
  {
  //Check if there is older object
  if( item != nullptr && isLibraryObjectOlderOrNone(item) ) {
    QString key = item->hashUidName();
    QFile fileHdr(FNAME_HDR);
    if( fileHdr.open(QIODevice::Append) ) {
      QWriteLocker locker( &mLock );
      SdLibraryHeader hdr;
      item->getHeader( hdr );
      SdLibraryReference ref( mReferenceMap.value(key) );
      //write header first
      ref.mHeaderPtr     = fileHdr.size();
      ref.mCreationTime  = hdr.mFileUid.mCreateTime;
      if( hdr.isPublic() )
        ref.mInsertionTimePublic = SvTime2x::current();
      else
        ref.mInsertionTimePrivate = SvTime2x::current();
      //Only for owner objects we allow uploading to server
      ref.mIsLocalChanged = !downloaded;

      QDataStream os( &fileHdr );
      hdr.write( os );
      fileHdr.close();

      //Check if there is older object
      if( mReferenceMap.contains(key) ) {
        //Object already in library and it is older, so we remove its file
        QFile::remove( fullPathOfLibraryObject( key ) );
        emit informationAppended( tr("Older object replaced %1").arg( item->getTitle() )  );
        mNewestMark = true;
        }
      else {
        emit informationAppended( tr("Object appended %1").arg( item->getTitle() )  );
        }


      mReferenceMap.insert( key, ref );
      mDirty = true;
      }
    }
  }




//!
//! \brief fullPath Return full path to library file
//! \param item     Item, for which path builded
//! \return         Full path to library file
//!
QString SdLibraryStorage::fullPath(const SdContainerFile *item) const
  {
  return fullPath( item->fileUid() );
  }







//!
//! \brief fullPath Return full path to library file
//! \param fileName Library file fileName
//! \return         Full path to library file
//!
QString SdLibraryStorage::fullPath(const QString &fileName) const
  {
  SvDir dir( mLibraryPath );
  return dir.slashedPath() + fileName.mid( 0, 2 ) + QStringLiteral("/") + fileName;
  }




//!
//! \brief fileNameOfLibraryObject Returns file name of library object
//! \param hashUidName             Unical id of object
//! \return                        File name of library object
//!
QString SdLibraryStorage::fileNameOfLibraryObject(const QString &hashUidName) const
  {
  if( !mReferenceMap.contains(hashUidName) )
    return QString{};
  return mReferenceMap.value(hashUidName).fileName( hashUidName );
  }




//!
//! \brief fullPathOfLibraryObject Returns full path to object in library
//! \param hashUidName             Unical id of object
//! \return                        Full path to file of object in library
//!
QString SdLibraryStorage::fullPathOfLibraryObject(const QString &hashUidName) const
  {
  return fullPath( fileNameOfLibraryObject(hashUidName) );
  }




bool SdLibraryStorage::isLibraryObjectFilePresent(const SdFileUid &fileUid) const
  {
  return QFile::exists( fullPath( fileUid.fileUid() ) );
  }




void SdLibraryStorage::saveDirtyCache()
  {
  if( mDirty ) {
    QWriteLocker locker( &mLock );
    QSaveFile file(FNAME_REF);
    if( file.open(QIODevice::WriteOnly) ) {
      QDataStream os( &file );
      os << REFERENCE_SIGNATURE << mReferenceMap << mAuthorAssoc << mCompliantDescrList << mPrivateLastSync << mPrivateLastList << mGlobalLastSync << mGlobalLastList << mAuthorLastList;
      if( file.commit() )
        mDirty = false;
      }
    }
  }



bool SdLibraryStorage::resetCache()
  {
  QFile file(FNAME_REF);
  if( !file.open( QIODevice::WriteOnly ) ) {
    mLibraryPath.clear();
    return false;
    }
  file.close();

  if( !mHeaderFile.open( QIODevice::WriteOnly ) ) {
    mLibraryPath.clear();
    return false;
    }
  mHeaderFile.write( "salixEdaLibraryHeaders", HDR_START );
  mHeaderFile.close();
  return true;
  }




//!
//! \brief libraryBuildStructure Builds structure of library
//!
void SdLibraryStorage::libraryBuildStructure()
  {
  QSettings s;
  mLibraryPath = s.value( QStringLiteral(SDK_LOCAL_LIBRARY_PATH) ).toString();
  if( mLibraryPath.isEmpty() || mLibraryPath.length() < 3 ) {
    //Library path not defined yet
    SvDir dir( QCoreApplication::applicationDirPath() );
    dir.cd( QStringLiteral("library/") );
    QDir().mkpath( dir.slashedPath() );
    mLibraryPath = dir.slashedPath();
    s.setValue( QStringLiteral(SDK_LOCAL_LIBRARY_PATH), mLibraryPath );
    }

  //Check library structure
  // Проверяем существование основной директории
  QDir baseDir(mLibraryPath);
  if( !baseDir.exists() )
    QDir().mkpath( mLibraryPath );

  //Creating subdirs x0-x7
  for( int i = 0; i < 8; ++i ) {
    QString dirName = QString("x%1").arg(i);
    QString dirPath = baseDir.filePath(dirName);

    QDir subDir(dirPath);
    //Create new subdir if it not exist
    if( !subDir.exists() )
      baseDir.mkdir(dirName);
    }


  //Close previously opened data base
  if( mHeaderFile.isOpen() ) {
    saveDirtyCache();
    mHeaderFile.close();
    }

  mHeaderFile.setFileName( FNAME_HDR );

  //Check if directory present
  QDir dir( cachePath() );
  if( !dir.exists() || (!QFile::exists(FNAME_REF) && !QFile::exists(FNAME_HDR) ) ) {
    //Directory not exist, create one
    if( !dir.exists() && !dir.mkpath( cachePath() ) ) {
      mLibraryPath.clear();
      return;
      }
    //Create data files
    if( !resetCache() )
      return;
    }

  //Check if file present
  if( !QFile::exists(FNAME_REF) || !QFile::exists(FNAME_HDR) ) {
    //One or more files are not exists - work is fail
    mLibraryPath.clear();
    return;
    }

  //Open headers and objects files
  if( !mHeaderFile.open( QIODevice::ReadOnly ) )
    mLibraryPath.clear();

  //Read reference file
  QFile file(FNAME_REF);
  if( file.open(QIODevice::ReadOnly) ) {
    QDataStream is( &file );
    QString signature;
    is >> signature >> mReferenceMap >> mAuthorAssoc >> mCompliantDescrList >> mPrivateLastSync >> mPrivateLastList >> mGlobalLastSync >> mGlobalLastList >> mAuthorLastList;
    if( signature != REFERENCE_SIGNATURE ) {

      }
    //os << mReferenceMap << mAuthorAssoc << mCompliantDescrList << mPrivateLastSync << mPrivateLastList << mGlobalLastSync << mGlobalLastList << mAuthorLastList;
    }
  else mLibraryPath.clear();

  if( !mLibraryPath.isEmpty() && !mPause ) {
    mScanTimer.start(20);
    }
  }






QByteArray SdLibraryStorage::objectGetFromFile(const QString &fileName) const
  {
  QString filePath = fullPath( fileName );
  if( QFile::exists(filePath) ) {
    QFile file(filePath);
    if( file.open(QIODevice::ReadOnly) )
      return file.readAll();
    }
  return QByteArray{};
  }





void SdLibraryStorage::objectDelete(const SdFileUid &fileUid)
  {
  QWriteLocker locker( &mLock );
  if( mReferenceMap.contains(fileUid.mHashUidName) ) {
    SdLibraryReference ref( mReferenceMap.value(fileUid.mHashUidName) );
    if( ref.isOlder(fileUid.mCreateTime) ) {
      //Remove library file
      QFile::remove( fullPathOfLibraryObject( fileUid.mHashUidName ) );

      //Remove reference to object
      mReferenceMap.remove( fileUid.mHashUidName );

      //Set dirty flag
      mDirty = true;
      }
    }
  }




void SdLibraryStorage::objectPut(const QByteArray &content)
  {
  QScopedPointer<SdContainerFile> item( sdObjectOnly<SdContainerFile>( SdObject::jsonCborCompressedFrom(content) )   );
  if( !item.isNull() )
    objectInsertPrivate( item.get(), true );
  }



QCborMap SdLibraryStorage::prepareQuery(int queryType, const SdFileUid &uid, bool appendObject) const
  {
  QReadLocker locker( &mLock );
  QCborMap map;
  map[SDRM_TYPE]              = queryType;
  map[SDRM_CLOUD_ID]          = privateCloudName();
  map[SDRM_AUTHOR_PUBLIC_KEY] = authorOfObject( uid.mHashUidName );
  map[SDRM_HASH_UID_NAME]     = uid.mHashUidName;
  map[SDRM_CREATE_TIME]       = uid.mCreateTime;
  map[SDRM_CONTENT]           = appendObject ? objectGetFromFile( uid.fileUid() ) : QByteArray{};
  return map;
  }



QCborMap SdLibraryStorage::prepareQueryList(int queryType, int time)
  {
  QCborMap map;
  map[SDRM_TYPE]        = queryType;
  map[SDRM_CLOUD_ID]    = privateCloudName();
  map[SDRM_CREATE_TIME] = time;
  return map;
  }




QString SdLibraryStorage::cachePath()
  {
  SvDir dir( QCoreApplication::applicationDirPath() );
  dir.cd( QStringLiteral("cache/") );
  return dir.slashedPath();
  }



void SdLibraryStorage::remoteSync(SdLibraryStorage *storage)
  {
  //Synchronization with private cloud
  //Synchronization is performed in two stages: upload updates since the last
  //sync to the cloud, and download changes since the last sync from the cloud.
  QSettings s;
  QList<SdFileUid> fileListForChange;
  qint32 curSync = SvTime2x::current();
  qint32 lastSync = storage->mPrivateLastSync;
  //First, collect objects that have changed since the last synchronization.
  //This includes newly inserted, updated, and deleted objects. Objects that
  //have been made public are also treated as deleted. Consider only private objects.
  storage->forEachReference( [lastSync,&fileListForChange] (const QString &hashUidName, const SdLibraryReference &ref) {
    if( !ref.isPublic() && ref.isLocalChanged() && ref.mInsertionTimePrivate > lastSync )
      fileListForChange.append( SdFileUid( hashUidName, ref.mCreationTime ) );
    });

  try {
    SdTcpCborClient client;
    client.openSocket(s.value(SDK_PRIVATE_CLOUD_IP, SD_DEFAULT_PRIVATE_CLOUD_IP).toString(), SD_PRIVATE_CLOUD_PORT );

    if( !fileListForChange.isEmpty() ) {
      //Now, sequentially query each object to check if an actual transfer is needed.
      //A transfer is required if the object in the cloud is an older version or does not exist at all.
      qDebug() << "Need to send to cloud objects:" << fileListForChange.count();
      //Perform sync for each object
      for( const auto &fileUid : std::as_const(fileListForChange) ) {
        //Check if we need break operation
        if( storage->mPause ) return;

        SdLibraryReference ref = storage->cfReference( fileUid.mHashUidName );
        if( ref.mCreationTime != fileUid.mCreateTime ) continue;

        if( ref.isLocalChanged() && ref.isInsertAfterPrivate(lastSync) ) {
          if( !ref.isRemovePrivate() && !storage->isLibraryObjectFilePresent( fileUid ) )
            //No real file for object, we skip that
            continue;
          //This object need to be sync
          qDebug() << "Check if need to send file:" << fileUid.mHashUidName;
          //Build "check" query
          QCborMap res = client.transferMap( storage->prepareQuery( SDRM_TYPE_CHECK, fileUid, false ) );
          if( res[SDRM_TYPE].toInteger() == SDRM_TYPE_GET ) {
            //Perform the actual object upload to the cloud. A deleted object is transmitted as an empty one.
            res = client.transferMap( storage->prepareQuery( SDRM_TYPE_OBJECT, fileUid, !ref.isRemovePrivate() ) );
            if( res[SDRM_TYPE].toInteger() != SDRM_TYPE_OK )
              return;
            storage->mIterCloudTransferOut++;
            }
          else if( res[SDRM_TYPE].toInteger() != SDRM_TYPE_OK )
            return;
          }
        }
      //Update sync time
      storage->mPrivateLastSync = curSync;
      storage->mDirty = true;
      }


    //Now, retrieve the list of server updates performed after the last synchronization.
    //This list will also include the objects uploaded in the previous step.
    QCborMap listMap = client.transferMap( prepareQueryList( SDRM_TYPE_GET_LIST, storage->mPrivateLastList ) );

    if( listMap[SDRM_TYPE].toInteger() == SDRM_TYPE_LIST ) {
      QCborArray ar = listMap[SDRM_LIST].toArray();
      fileListForChange.clear();
      SdFileUid uid;
      //For each item in the list, check if the object in the cloud
      //is newer than our local copy. If it is, request the object itself.
      for( auto const &val : ar ) {
        if( uid.fromFileUid(val.toString()) )
          fileListForChange.append( uid );
        }

      if( !fileListForChange.isEmpty() ) {
        //Perform sync for each object
        for( const auto &fileUid : std::as_const(fileListForChange) ) {
          //Check if we need break operation
          if( storage->mPause ) return;

          if( storage->isLibraryObjectOlderOrNone( fileUid ) ) {
            QCborMap res = client.transferMap( storage->prepareQuery( SDRM_TYPE_GET, fileUid, false ) );
            if( res[SDRM_TYPE].toInteger() == SDRM_TYPE_OBJECT ) {
              //Object received. An empty object indicates that the object has been deleted.
              QByteArray fileContent = res[SDRM_CONTENT].toByteArray();
              if( fileContent.isEmpty() ) {
                //Delete from library
                storage->objectDelete( fileUid );
                }
              else {
                storage->objectPut( fileContent );
                storage->mIterCloudTransferIn++;
                }
              }
            else return;
            }
          }
        }
      //Update last list time
      storage->mPrivateLastList = listMap[SDRM_CREATE_TIME].toInteger();
      storage->mDirty = true;
      }
    }
  catch(const std::exception& e) {
    qDebug() << "Error occured" << e.what();
    storage->mCloudError = e.what();
    }





  //Now perform synchronization with the public cloud. It is done exactly the
  //same as with the private cloud, except that public objects are collected.
  fileListForChange.clear();
  curSync = SvTime2x::current();
  lastSync = storage->mGlobalLastSync;
  storage->forEachReference( [lastSync,&fileListForChange] (const QString &hashUidName, const SdLibraryReference &ref) {
    if( ref.isPublic() && ref.isLocalChanged() && ref.mInsertionTimePublic > lastSync )
      fileListForChange.append( SdFileUid( hashUidName, ref.mCreationTime ) );
    });

  try {
    SdTcpCborClient client;
    client.openSocket( s.value(SDK_GLOBAL_STORAGE_IP, SD_DEFAULT_GLOBAL_STORAGE_IP).toString(), SD_GLOBAL_STORAGE_PORT );

    if( !fileListForChange.isEmpty() ) {
      //Perform sync for each object
      for( const auto &fileUid : std::as_const(fileListForChange) ) {
        //Check if we need break operation
        if( storage->mPause ) return;

        SdLibraryReference ref = storage->cfReference( fileUid.mHashUidName );
        if( ref.mCreationTime != fileUid.mCreateTime ) continue;

        if( ref.isLocalChanged() && ref.isInsertAfterPublic(lastSync) ) {
          if( !ref.isRemovePublic() && !storage->isLibraryObjectFilePresent( fileUid ) )
            //No real file for object, we skip that
            continue;
          //This object need to be sync
          //Build "check" query
          QCborMap res = client.transferMap( storage->prepareQuery( SDRM_TYPE_CHECK, fileUid, false ) );
          if( res[SDRM_TYPE].toInteger() == SDRM_TYPE_GET ) {
            //Send object
            res = client.transferMap( storage->prepareQuery( SDRM_TYPE_OBJECT, fileUid, !ref.isRemovePublic() ) );
            if( res[SDRM_TYPE].toInteger() != SDRM_TYPE_OK )
              return;
            storage->mIterGlobalTransferOut++;
            }
          }
        }

      //Update sync time
      storage->mGlobalLastSync = curSync;
      storage->mDirty = true;
      }

    //Get from public library list newest objects
    QCborMap listMap = client.transferMap( prepareQueryList( SDRM_TYPE_GET_LIST, storage->mGlobalLastList ) );

    if( listMap[SDRM_TYPE].toInteger() == SDRM_TYPE_LIST ) {
      QCborArray ar = listMap[SDRM_LIST].toArray();
      fileListForChange.clear();
      SdFileUid uid;
      for( auto const &val : ar ) {
        if( uid.fromFileUid(val.toString()) )
          fileListForChange.append( uid );
        }

      if( !fileListForChange.isEmpty() ) {
        //Perform sync for each object
        for( const auto &fileUid : std::as_const(fileListForChange) ) {
          //Check if we need break operation
          if( storage->mPause ) return;

          if( storage->isLibraryObjectOlderOrNone( fileUid ) ) {
            QCborMap res = client.transferMap( storage->prepareQuery( SDRM_TYPE_GET, fileUid, false ) );
            if( res[SDRM_TYPE].toInteger() == SDRM_TYPE_OBJECT ) {
              QByteArray fileContent = res[SDRM_CONTENT].toByteArray();
              if( fileContent.isEmpty() ) {
                //Delete from library
                storage->objectDelete( fileUid );
                }
              else {
                storage->objectPut( fileContent );
                storage->mIterGlobalTransferIn++;
                }
              }
            else return;
            }
          }
        }
      //Update last list time
      storage->mGlobalLastList = listMap[SDRM_CREATE_TIME].toInteger();
      storage->mDirty = true;
      }


    //Author synchronization. Retrieve the list of authors whose data has changed since the last
    //synchronization from the server. For each author in the list, update the local author map.
    listMap = client.transferMap( prepareQueryList( SDRM_TYPE_GET_AUTHOR_LIST, storage->mAuthorLastList ) );
    if( listMap[SDRM_TYPE].toInteger() == SDRM_TYPE_AUTHOR_LIST ) {
      QCborArray ar = listMap[SDRM_LIST].toArray();
      QWriteLocker locker( &storage->mLock );
      for( auto const &val : ar ) {
        QCborMap author = val.toMap();
        QString authorPublicKey = author[SDRM_AUTHOR_PUBLIC_KEY].toString();
        QString authorName      = author[SDRM_AUTHOR_NAME].toString();
        int authorTotalCount    = author[SDRM_AUTH_TOTAL_COUNT].toInteger();
        int authorComplCount    = author[SDRM_AUTH_COMPL_COUNT].toInteger();
        int authorTotalComplCount = author[SDRM_AUTH_TOTAL_COMPL_COUNT].toInteger();
        storage->mAuthorAssoc[authorPublicKey].update( authorName, authorTotalCount, authorComplCount, authorTotalComplCount );
        }

      //Update last author list time
      storage->mAuthorLastList = listMap[SDRM_CREATE_TIME].toInteger();
      storage->mDirty = true;
      }


    //Uploading compliants. Prepare and send the list of compliants on components.
    //If the upload is successful, remove the transmitted compliants from the local list.
    QCborArray ar;
    auto complList = storage->mCompliantDescrList;
    if( complList.count() ) {
      for( auto const &comp : std::as_const(complList) ) {
        //Build single compliant
        QCborMap comap;
        comap[SDRM_HASH_UID_NAME]     = comp.mFileUid.mHashUidName;
        comap[SDRM_CREATE_TIME]       = comp.mFileUid.mCreateTime;
        comap[SDRM_COMPLIANT_MESSAGE] = comp.mMessage;

        //Append compliant to list
        ar.append( comap );
        }

      //Build query
      QCborMap map;
      map[SDRM_TYPE]              = SDRM_TYPE_COMPLIANT_LIST;
      map[SDRM_AUTHOR_PUBLIC_KEY] = authorPublicKey();
      map[SDRM_LIST]              = ar;

      listMap = client.transferMap( map );
      if( listMap[SDRM_TYPE].toInteger() == SDRM_TYPE_OK ) {
        //List sended successfully, so remove sended records
        QWriteLocker locker( &storage->mLock );
        storage->mCompliantDescrList.remove( 0, complList.count() );
        storage->mDirty = true;
        }
      }

    }
  catch(const std::exception& e) {
    qDebug() << "Error occured" << e.what();
    storage->mGlobalError = e.what();
    }
  }










