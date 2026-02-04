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


#define FNAME_REF    cachePath() + QStringLiteral("reference.dat")
#define FNAME_HDR    cachePath() + QStringLiteral("headers.dat")
#define HDR_START    23

SdLibraryStorage::SdLibraryStorage() :
  QObject(),
  mReferenceMap(),
  mHeaderFile(),
  mDirty(false),
  mUploadAvailable(false),
  mNewestMark(false),
  mLock()
  {
  connect( &mScanTimer, &QTimer::timeout, this, &SdLibraryStorage::periodicScan );
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
    s.setValue( QStringLiteral("LIBRARY_PATH"), mLibraryPath );

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
    mExistList.clear();
    mUploadAvailable = false;

    libraryBuildStructure();
    }
  }




//!
//! \brief libraryInit Init library system
//!
void SdLibraryStorage::libraryInit()
  {
  QWriteLocker locker( &mLock );

  libraryBuildStructure();
  }




//!
//! \brief libraryComplete Complete work of libraryStorage. We flush caches and delete libraryStorage
//!
void SdLibraryStorage::libraryComplete()
  {
  if( mDirty ) {
    QWriteLocker locker( &mLock );
    QSaveFile file(FNAME_REF);
    if( file.open(QIODevice::WriteOnly) ) {
      QDataStream os( &file );
      os << mReferenceMap;
      if( file.commit() )
        mDirty = false;
      }
    }

  //Close previously opened data base
  if( mHeaderFile.isOpen() )
    mHeaderFile.close();

  deleteLater();
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
  if( item != nullptr && item->getAuthorKey() == item->getDefaultAuthor() ) {
    QWriteLocker locker( &mLock );
    QString uid = item->hashUidName();
    if( mReferenceMap.contains(uid) && !mReferenceMap.value(uid).isRemoveAny() ) {
      //Mark as need to be deleted from server
      mReferenceMap[uid].markAsRemoved();

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
  if( mReferenceMap.contains( hashUidName ) ) {
    //Load object
    return sdObjectOnly<SdContainerFile>( SdObject::fileJsonLoad( fullPathOfLibraryObject(hashUidName) ) );
    }
  return nullptr;
  }







//!
//! \brief cfIsOlder   Test if object which represents by hashUidName and time present in library and older than there is in library
//! \param hashUidName hashUidName of tested object
//! \param time        time of locking of tested object
//! \return            true if tested object present in library and it older then in library
//!
bool SdLibraryStorage::cfIsOlder(const QString hashUidName, qint32 time) const
  {
  QReadLocker locker( &mLock );
  return mReferenceMap.contains( hashUidName ) && mReferenceMap.value(hashUidName).isNewer( time );
  }



//!
//! \brief cfIsOlder Overloaded function. Test if object present in library and older than there is in library
//! \param item      Tested object
//! \return          true if tested object present in library and it older then in library
//!
bool SdLibraryStorage::cfIsOlder(const SdContainerFile *item) const
  {
  if( item == nullptr ) return false;
  return cfIsOlder( item->hashUidName(), item->getTime() );
  }




//!
//! \brief cfIsOlderOrSame Test if object which represents by hashUidName and time present in library and older or same than there is in library
//! \param hashUidName     hashUidName of tested object
//! \param time            time of locking of tested object
//! \return                true if tested object present in library and it older then in library
//!
bool SdLibraryStorage::cfIsOlderOrSame(const QString hashUidName, qint32 time) const
  {
  QReadLocker locker( &mLock );
  return mReferenceMap.contains( hashUidName ) && mReferenceMap.value(hashUidName).isNewerOrSame( time );
  }




//!
//! \brief cfIsOlderOrSame Overloaded function. Test if object present in library and older or same than there is in library
//! \param item            Tested object
//! \return                true if tested object present in library and it older then in library
//!
bool SdLibraryStorage::cfIsOlderOrSame(const SdContainerFile *item) const
  {
  if( item == nullptr ) return false;
  return cfIsOlderOrSame( item->hashUidName(), item->getTime() );
  }



bool SdLibraryStorage::cfObjectContains(const QString hashUidName) const
  {
  QReadLocker locker( &mLock );
  return mReferenceMap.contains(hashUidName) && !mReferenceMap.value(hashUidName).isRemoved();
  }




void SdLibraryStorage::cfDeleteReset(const QString hashUidName, quint8 deleteMask)
  {
  QWriteLocker locker( &mLock );
  if( !mReferenceMap.contains(hashUidName) ) return;
  if( mReferenceMap[hashUidName].mFlags & deleteMask )
    mReferenceMap[hashUidName].mFlags &= ~deleteMask;
  if( mReferenceMap[hashUidName].isCanBeRemove() )
    mReferenceMap.remove( hashUidName );
  }




SdLibraryReference SdLibraryStorage::cfReference(const QString &hashUidName) const
  {
  QReadLocker locker( &mLock );
  return mReferenceMap.value( hashUidName );
  }




bool SdLibraryStorage::cfIsPresentAndPrivate(const QString &hashUidName) const
  {
  QReadLocker locker( &mLock );
  if( !mReferenceMap.contains(hashUidName) )
    return false;
  return mReferenceMap.value(hashUidName).isPrivate();
  }





//!
//! \brief header      Get header of object
//! \param hashUidName object unical identificator
//! \param hdr         place to receiv object header
//! \return            true if header readed successfully
//!
bool SdLibraryStorage::header(const QString hashUidName, SdLibraryHeader &hdr)
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




QString SdLibraryStorage::authorGlobalName(const QString &authorPublicKey) const
  {
  QReadLocker locker( &mLock );
  return mAuthorAssoc.value(authorPublicKey, tr("Anonim") );
  }





//!
//! \brief periodicScan Perform scan step
//!
void SdLibraryStorage::periodicScan()
  {
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

      if( mExistList.contains( fileName ) )
        //File exist, we remove it from existing files because it processed
        mExistList.remove( fileName );
      else {
        //File not exist, we append it to reference
        //Load object
        //Check if file actually exist and not have been removed
        if( QFile::exists(file.absoluteFilePath()) ) {
          QScopedPointer<SdContainerFile> item( sdObjectOnly<SdContainerFile>(SdObject::fileJsonLoad(file.absoluteFilePath())) );
          //Fix crash: check if item readed correctly
          if( item.isNull() ) continue;
          //Check if this object is older then in library present
          if( cfIsOlder( item.get() ) ) {
            //Remove this file because it is older than in library
            QFile::remove( file.absoluteFilePath() );
            emit informationAppended( tr("Older object removed %1").arg( item->getTitle() )  );
            }
          else {
            //Append object to reference
            insertReferenceAndHeader( item.get(), false );
            }
          }
        }

      }

    if( mScanList.count() == 0 ) {
      mScanTimer.stop();
      //Scan is complete
      //Remain files in existing files map are deleted files
      //we remove its records from library reference
      int removeCount = 0;
      for( auto it = mExistList.cbegin(); it != mExistList.cend(); it++ ) {
        //Check if file actually removed
        if( !QFile::exists( fullPathOfLibraryObject( it.value() ) ) ) {
          qDebug() << it.value() << fullPathOfLibraryObject( it.value() );
          mReferenceMap.remove( it.value() );
          removeCount++;
          }
        }
      if( removeCount )
        emit informationAppended( tr("Remove external deleted objects %1").arg( removeCount )  );

      //Perform sync with remote storage
  //    QtConcurrent::run( SdLibraryStorage::remoteSync, this ).then( this, [this] () {
        mScanTimer.start( 30000 );
        //});

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

    //Fill map for existing files
    mExistList.clear();
    for( auto it = mReferenceMap.cbegin(); it != mReferenceMap.cend(); it++ )
      mExistList.insert( fileNameOfLibraryObject( it.key() ), it.key() );

    mScanTimer.start( 100 );
    }
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
  if( mLibraryPath.isEmpty() || item == nullptr || item->isEditEnable() || cfIsOlderOrSame( item ) )
    return;

  //Insert reference and header
  insertReferenceAndHeader( item, downloaded );

  //Insert in library
  QString filePath = fullPath( item );

  item->fileJsonSave( filePath );
  }




//!
//! \brief insertReferenceAndHeader Insert in cache reference to object and header of object
//! \param item                     Object which inserted
//!
void SdLibraryStorage::insertReferenceAndHeader(const SdContainerFile *item, bool downloaded)
  {
  //Check if there is older object
  if( item != nullptr && !cfIsOlderOrSame(item) ) {
    QString key = item->hashUidName();
    bool isPresentAndPrivate = cfIsPresentAndPrivate( key );
    QFile fileHdr(FNAME_HDR);
    if( fileHdr.open(QIODevice::Append) ) {
      QWriteLocker locker( &mLock );
      SdLibraryHeader hdr;
      item->getHeader( hdr );
      SdLibraryReference ref;
      //write header first
      ref.mHeaderPtr     = fileHdr.size();
      ref.mCreationTime  = hdr.mFileUid.mCreateTime;
      ref.mInsertionTime = SvTime2x::current();
      //Only for owner objects we allow uploading to server
      ref.mFlags         = downloaded ? 0 : SDLR_LOCAL_CHANGED;
      if( item->isPublic() && isPresentAndPrivate )
        ref.mFlags |= SDLR_REMOVE_PRIVATE;
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




//!
//! \brief libraryBuildStructure Builds structure of library
//!
void SdLibraryStorage::libraryBuildStructure()
  {
  QSettings s;
  mLibraryPath = s.value( QStringLiteral("LIBRARY_PATH") ).toString();
  if( mLibraryPath.isEmpty() || mLibraryPath.length() < 3 ) {
    //Library path not defined yet
    SvDir dir( QCoreApplication::applicationDirPath() );
    dir.cd( QStringLiteral("library/") );
    QDir().mkpath( dir.slashedPath() );
    mLibraryPath = dir.slashedPath();
    s.setValue( QStringLiteral("LIBRARY_PATH"), mLibraryPath );
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
  if( mHeaderFile.isOpen() )
    mHeaderFile.close();

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
    QFile file(FNAME_REF);
    if( !file.open( QIODevice::WriteOnly ) ) {
      mLibraryPath.clear();
      return;
      }
    file.close();

    if( !mHeaderFile.open( QIODevice::WriteOnly ) ) {
      mLibraryPath.clear();
      return;
      }
    mHeaderFile.write( "salixEdaLibraryHeaders", HDR_START );
    mHeaderFile.close();
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
    is >> mReferenceMap;
    }
  else mLibraryPath.clear();

  if( !mLibraryPath.isEmpty() ) {
    mScanTimer.start(20);
    }
  }




QByteArray SdLibraryStorage::objectGet(const QString &hashUidName) const
  {
  return objectGetFromFile( fileNameOfLibraryObject(hashUidName) );
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





void SdLibraryStorage::objectDelete(const QString &hashUidName)
  {
  QWriteLocker locker( &mLock );
  mReferenceMap.remove( hashUidName );
  mDirty = true;
  }




void SdLibraryStorage::objectPut(const QByteArray &content)
  {
  QScopedPointer<SdContainerFile> item( sdObjectOnly<SdContainerFile>( SdObject::jsonCborCompressedFrom(content) )   );
  if( !item.isNull() )
    objectInsertPrivate( item.get(), true );
  }


//Fields of query
#define SDRM_TYPE          0
#define SDRM_HASH_UID_NAME 1
#define SDRM_CREATE_TIME   2
#define SDRM_CONTENT       3
#define SDRM_LIST          4

#define SDRM_TYPE_FAIL     0
#define SDRM_TYPE_OK       1 //!< OK Accnowledge
#define SDRM_TYPE_CHECK    2 //!< Check cloud storage is need to sync object or not
#define SDRM_TYPE_GET      3 //!<
#define SDRM_TYPE_REMOVE   4 //!< Remove file from cloud
#define SDRM_TYPE_OBJECT   5
#define SDRM_TYPE_GET_LIST 6 //!< Query list of newest files
#define SDRM_TYPE_LIST     7


QCborMap SdLibraryStorage::prepareQuery(int queryType, const SdFileUid &uid, bool appendObject) const
  {
  QReadLocker locker( &mLock );
  QCborMap map;
  map[SDRM_TYPE]          = queryType;
  map[SDRM_HASH_UID_NAME] = uid.mHashUidName;
  map[SDRM_CREATE_TIME]   = uid.mCreateTime;
  if( appendObject )
    map[SDRM_CONTENT] = objectGetFromFile( uid.fileUid() );
  return map;
  }



QCborMap SdLibraryStorage::prepareQueryList(int queryType, int time)
  {
  QCborMap map;
  map[SDRM_TYPE]        = queryType;
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
  //Perform local sync
  QSettings s;
  QList<SdFileUid> fileListForChange;
  qint32 curSync = SvTime2x::current();
  qint32 lastSync = s.value( SDK_PRIVATE_LAST_SYNC, 0 ).toInt();
  storage->forEachReference( [lastSync,&fileListForChange] (const QString &hashUidName, const SdLibraryReference &ref) {
    if( !ref.isPublic() && ref.mInsertionTime > lastSync )
      fileListForChange.append( SdFileUid( hashUidName, ref.mCreationTime ) );
    });

  try {
    SdTcpCborClient client;
    client.openSocket("host", 1234);

    if( !fileListForChange.isEmpty() ) {
      //Perform sync for each object
      for( auto fileUid : fileListForChange ) {
        SdLibraryReference ref = storage->cfReference( fileUid.mHashUidName );
        if( ref.mCreationTime != fileUid.mCreateTime ) continue;

        if( ref.isValid() && ref.isLocalChanged() && ref.isInsertAfter(lastSync) ) {
          //This object need to be sync
          if( ref.isRemovePrivate() ) {
            //Build "remove" query
            QCborMap res = client.transferMap( storage->prepareQuery( SDRM_TYPE_REMOVE, fileUid ) );
            if( res[SDRM_TYPE].toInteger() == SDRM_TYPE_OK )
              storage->cfDeleteReset( fileUid.mHashUidName, SDLR_REMOVE_PRIVATE );
            else return;
            }
          else {
            //Build "check" query
            QCborMap res = client.transferMap( storage->prepareQuery( SDRM_TYPE_CHECK, fileUid ) );
            if( res[SDRM_TYPE].toInteger() == SDRM_TYPE_GET ) {
              //Send object
              res = client.transferMap( storage->prepareQuery( SDRM_TYPE_OBJECT, fileUid, true ) );
              if( res[SDRM_TYPE].toInteger() != SDRM_TYPE_OK )
                return;
              }
            }
          }
        }
      }

    //Update sync time
    s.setValue( SDK_PRIVATE_LAST_SYNC, curSync );

    //Get from cloud list newest objects
    QCborMap listMap = client.transferMap( prepareQueryList( SDRM_TYPE_GET_LIST, s.value( SDK_PRIVATE_LAST_LIST, 0 ).toInt() ) );

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
        for( auto fileUid : fileListForChange ) {
          if( storage->cfIsOlderOrSame( fileUid.mHashUidName, fileUid.mCreateTime ) )
            continue;
          QCborMap res = client.transferMap( storage->prepareQuery( SDRM_TYPE_GET, fileUid ) );
          if( res[SDRM_TYPE].toInteger() == SDRM_TYPE_OBJECT ) {
            QByteArray fileContent = res[SDRM_CONTENT].toByteArray();
            if( fileContent.isEmpty() ) {
              //Delete from library
              storage->objectDelete( fileUid.mHashUidName );
              }
            else {
              storage->objectPut( fileContent );
              }
            }
          else return;
          }
        }
      //Update last list time
      int lastListTime = listMap[SDRM_CREATE_TIME].toInteger();
      s.setValue( SDK_PRIVATE_LAST_LIST, lastListTime );
      }
    }
  catch(const std::exception& e) {
    qDebug() << "Error occured" << e.what();
    }






  fileListForChange.clear();
  curSync = SvTime2x::current();
  lastSync = s.value( SDK_GLOBAL_LAST_SYNC, 0 ).toInt();
  storage->forEachReference( [lastSync,&fileListForChange] (const QString &hashUidName, const SdLibraryReference &ref) {
    if( ref.isPublic() && ref.mInsertionTime > lastSync )
      fileListForChange.append( SdFileUid( hashUidName, ref.mCreationTime ) );
    });

  try {
    SdTcpCborClient client;
    client.openSocket("cloud", 1234);

    if( !fileListForChange.isEmpty() ) {
      //Perform sync for each object
      for( auto fileUid : fileListForChange ) {
        SdLibraryReference ref = storage->cfReference( fileUid.mHashUidName );
        if( ref.mCreationTime != fileUid.mCreateTime ) continue;

        if( ref.isValid() && ref.isLocalChanged() && ref.isInsertAfter(lastSync) ) {
          //This object need to be sync
          if( ref.isRemoveGlobal() ) {
            //Build "remove" query
            QCborMap res = client.transferMap( storage->prepareQuery( SDRM_TYPE_REMOVE, fileUid ) );
            if( res[SDRM_TYPE].toInteger() == SDRM_TYPE_OK )
              storage->cfDeleteReset( fileUid.mHashUidName, SDLR_REMOVE_PRIVATE );
            else return;
            }
          else {
            //Build "check" query
            QCborMap res = client.transferMap( storage->prepareQuery( SDRM_TYPE_CHECK, fileUid ) );
            if( res[SDRM_TYPE].toInteger() == SDRM_TYPE_GET ) {
              //Send object
              res = client.transferMap( storage->prepareQuery( SDRM_TYPE_OBJECT, fileUid, true ) );
              if( res[SDRM_TYPE].toInteger() != SDRM_TYPE_OK )
                return;
              }
            }
          }
        }
      }

    //Update sync time
    s.setValue( SDK_GLOBAL_LAST_SYNC, curSync );

    //Get from cloud list newest objects
    QCborMap listMap = client.transferMap( prepareQueryList( SDRM_TYPE_GET_LIST, s.value( SDK_GLOBAL_LAST_LIST, 0 ).toInt() ) );

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
        for( auto fileUid : fileListForChange ) {
          if( storage->cfIsOlderOrSame( fileUid.mHashUidName, fileUid.mCreateTime ) )
            continue;
          QCborMap res = client.transferMap( storage->prepareQuery( SDRM_TYPE_GET, fileUid ) );
          if( res[SDRM_TYPE].toInteger() == SDRM_TYPE_OBJECT ) {
            QByteArray fileContent = res[SDRM_CONTENT].toByteArray();
            if( fileContent.isEmpty() ) {
              //Delete from library
              storage->objectDelete( fileUid.mHashUidName );
              }
            else {
              storage->objectPut( fileContent );
              }
            }
          else return;
          }
        }
      //Update last list time
      int lastListTime = listMap[SDRM_CREATE_TIME].toInteger();
      s.setValue( SDK_GLOBAL_LAST_LIST, lastListTime );
      }
    }
  catch(const std::exception& e) {
    qDebug() << "Error occured" << e.what();
    }
  }










