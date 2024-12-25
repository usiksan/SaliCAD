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
#include "objects/SdObjectNetClient.h"
#include "SvLib/SvDir.h"

#include <QDir>
#include <QReadLocker>
#include <QWriteLocker>
#include <QSaveFile>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>
#include <QDebug>

#define FNAME_REF    cachePath() + QStringLiteral("reference.dat")
#define FNAME_HDR_V1 cachePath() + QStringLiteral("headers.dat")
#define FNAME_HDR    cachePath() + QStringLiteral("headersV2.dat")
#define HDR_START    18

SdLibraryStorage::SdLibraryStorage() :
  QObject(),
  mReferenceMap(),
  mHeaderFile(),
  mNetClientLocker(nullptr),
  mDirty(false),
  mUploadAvailable(false),
  mNewestMark(false),
  mLock()
  {
  }





//!
//! \brief libraryPathSet Set new library path
//! \param path           New library path
//!
//! Current caches are cleared, library path changed and scan start from begin
void SdLibraryStorage::libraryPathSet(const QString &path)
  {
  QWriteLocker locker( &mLock );

  //Build new library directory
  SvDir dir( path );
  QDir().mkpath( dir.slashedPath() );
  mLibraryPath = dir.slashedPath();
  QSettings s;
  s.setValue( QStringLiteral("LIBRARY_PATH"), mLibraryPath );

  //Clear cache
  mReferenceMap.clear();
  QFile file(FNAME_HDR);
  if( file.open( QIODevice::WriteOnly ) ) {
    file.resize(HDR_START);
    file.close();
    }

  //Clear current scan process
  mScanList.clear();
  mExistList.clear();
  mUploadAvailable = false;
  if( mNetClientLocker == nullptr )
    mNetClientLocker = new SdNetClientLocker();
  }




//!
//! \brief libraryInit Init library system
//!
void SdLibraryStorage::libraryInit()
  {
  QWriteLocker locker( &mLock );

  if( mNetClientLocker == nullptr )
    mNetClientLocker = new SdNetClientLocker();

  QSettings s;
  mLibraryPath = s.value( QStringLiteral("LIBRARY_PATH") ).toString();
  if( mLibraryPath.isEmpty() || mLibraryPath.length() < 3 ) {
    //Library path not defined yet
    SvDir dir( QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation).at(0) );
    dir.cd( QStringLiteral("library/") );
    QDir().mkpath( dir.slashedPath() );
    mLibraryPath = dir.slashedPath();
    s.setValue( QStringLiteral("LIBRARY_PATH"), mLibraryPath );
    }

  //Close previously opened data base
  if( mHeaderFile.isOpen() )
    mHeaderFile.close();

  //Delete Header file of previous version
  if( QFile::exists(FNAME_HDR_V1) )
    QFile::remove( FNAME_HDR_V1 );

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
    mHeaderFile.write( "saliLibraryHeaders", HDR_START );
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
    connect( &mScanTimer, &QTimer::timeout, this, &SdLibraryStorage::periodicScan );
    mScanTimer.start(20);
    }
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
  //If inserted object is older than present in library then nothing done
  if( item == nullptr || item->isEditEnable() || cfIsOlderOrSame( item ) )
    return;

  //Insert reference and header
  insertReferenceAndHeader( item );

  //Insert in library
  QString fname = item->getLibraryFName();
  SvDir dir( mLibraryPath );
  QDir().mkpath( dir.slashedPath() + fname.left(2) );

  item->fileJsonSave( dir.slashedPath() + fname );
  }




void SdLibraryStorage::cfObjectDelete(const SdContainerFile *item)
  {
  if( item != nullptr && item->getAuthor() == item->getDefaultAuthor() ) {
    QString uid = item->getUid();
    if( mReferenceMap.contains(uid) && !mReferenceMap.value(uid).isNeedDelete() ) {
      //Mark as need to be deleted from server
      mReferenceMap[uid].deleteSet();

      //Delete object file
      QFile::remove( fullPath( item->getLibraryFName() ) );

      mDirty = true;
      }
    }
  }





//!
//! \brief cfObjectGet Load object from library
//! \param uid         UID of loaded object
//! \return            Loaded object or nullptr
//!
SdContainerFile *SdLibraryStorage::cfObjectGet(const QString uid) const
  {
  if( mReferenceMap.contains( uid ) ) {
    //Load object
    return sdObjectOnly<SdContainerFile>( SdObject::fileJsonLoad( fullPathOfLibraryObject(uid) ) );
    }
  return nullptr;
  }





//!
//! \brief cfObjectUpload Return object available to uploading to remote server
//! \return               Object to uploading
//!
SdContainerFile *SdLibraryStorage::cfObjectUpload() const
  {
  //Scan reference map to find object to uploading
  auto it = mReferenceMap.cbegin();
  while( it != mReferenceMap.cend() )
    if( it.value().isNeedUpload() )
      break;
    else
      it++;

  if( it == mReferenceMap.cend() )
    //No objects to uploading
    return nullptr;
  return cfObjectGet( it.key() );
  }




void SdLibraryStorage::cfObjectUploaded(const QString uid)
  {
  if( mReferenceMap.contains(uid) ) {
    mReferenceMap[uid].uploadReset();
    mDirty = true;
    }
  }




//!
//! \brief cfIsOlder Test if object which represents by uid and time present in library and older than there is in library
//! \param uid       uid of tested object
//! \param time      time of locking of tested object
//! \return          true if tested object present in library and it older then in library
//!
bool SdLibraryStorage::cfIsOlder(const QString uid, qint32 time) const
  {
  QReadLocker locker( &mLock );
  return mReferenceMap.contains( uid ) && mReferenceMap.value(uid).isNewer( time );
  }



//!
//! \brief cfIsOlder Overloaded function. Test if object present in library and older than there is in library
//! \param item      Tested object
//! \return          true if tested object present in library and it older then in library
//!
bool SdLibraryStorage::cfIsOlder(const SdContainerFile *item) const
  {
  if( item == nullptr ) return false;
  return cfIsOlder( item->getUid(), item->getTime() );
  }




//!
//! \brief cfIsOlderOrSame Test if object which represents by uid and time present in library and older or same than there is in library
//! \param uid             uid of tested object
//! \param time            time of locking of tested object
//! \return                true if tested object present in library and it older then in library
//!
bool SdLibraryStorage::cfIsOlderOrSame(const QString uid, qint32 time) const
  {
  QReadLocker locker( &mLock );
  return mReferenceMap.contains( uid ) && mReferenceMap.value(uid).isNewerOrSame( time );
  }




//!
//! \brief cfIsOlderOrSame Overloaded function. Test if object present in library and older or same than there is in library
//! \param item            Tested object
//! \return                true if tested object present in library and it older then in library
//!
bool SdLibraryStorage::cfIsOlderOrSame(const SdContainerFile *item) const
  {
  if( item == nullptr ) return false;
  return cfIsOlderOrSame( item->getUid(), item->getTime() );
  }





//!
//! \brief header Get header of object
//! \param uid    object unical identificator
//! \param hdr    place to receiv object header
//! \return       true if header readed successfully
//!
bool SdLibraryStorage::header(const QString uid, SdLibraryHeader &hdr)
  {
  //For empty key return false to indicate no header
  if( uid.isEmpty() ) return false;

  QWriteLocker locker( &mLock );
  if( !mReferenceMap.contains(uid) ) return false;
  mHeaderFile.seek( mReferenceMap.value(uid).mHeaderPtr );
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
    if( mReferenceMap.value(hdr.uid()).mHeaderPtr == position ) {
      if( fun1( hdr) )
        return true;
      }
    }
  return false;
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
    //qDebug() << subdr;
    QFileInfoList fileList = QDir(subdr).entryInfoList( QDir::Files );

    //Subdirectory name
    QString prefix( subdir.baseName() );
    if( !prefix.endsWith( QChar('/') ) )
      prefix.append( QChar('/') );

    for( const auto &file : fileList ) {
      QString fileName = prefix + file.fileName();

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
            //Sometime file name is different from file name returned by fullPath()
            //in this case we rename file
            QString theFullPath( fullPath( item->getLibraryFName() )  );
            if( file.absoluteFilePath() != theFullPath  ) {
              qDebug() << "Different name" << file.absoluteFilePath() << theFullPath;
              SvDir dir( mLibraryPath );
              QDir().mkpath( dir.slashedPath() + item->getLibraryFName().left(2) );

              QFile::rename( file.absoluteFilePath(), theFullPath );
              }

            //Append object to reference
            insertReferenceAndHeader( item.get() );
            }
          }
        }

      }

    if( mScanList.count() == 0 ) {
      //Scan is complete
      //Remain files in existing files map are deleted files
      //we remove its records from library reference
      int removeCount = 0;
      for( auto it = mExistList.cbegin(); it != mExistList.cend(); it++ ) {
        //Check if file actually removed
        if( !QFile::exists( fullPath( SdContainerFile::getLibraryFName( it.value(), mReferenceMap.value(it.value()).mCreationTime ) ) ) ) {
          qDebug() << it.value() << fullPath( SdContainerFile::getLibraryFName( it.value(), mReferenceMap.value(it.value()).mCreationTime ) );
          mReferenceMap.remove( it.value() );
          removeCount++;
          }
        }
      if( removeCount )
        emit informationAppended( tr("Remove external deleted objects %1").arg( removeCount )  );

      //We can begin sync with remote server
      if( mNetClientLocker != nullptr ) {
        delete mNetClientLocker;
        mNetClientLocker = nullptr;
        }

      if( mNewestMark ) {
        mNewestMark = false;
        emit updateNewestMark();
        }

      mScanTimer.start( 5000 );
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

    if( mScanList.count() == 0 ) {
      //Nothing to scan
      //We can begin sync with remote server
      if( mNetClientLocker != nullptr ) {
        delete mNetClientLocker;
        mNetClientLocker = nullptr;
        }
      }
    mScanTimer.start( 50 );
    }
  }




//!
//! \brief insertReferenceAndHeader Insert in cache reference to object and header of object
//! \param item                     Object which inserted
//!
void SdLibraryStorage::insertReferenceAndHeader(const SdContainerFile *item)
  {
  if( item != nullptr && !cfIsOlderOrSame(item) ) {
    //Check if there is older object
    QFile fileHdr(FNAME_HDR);
    if( fileHdr.open(QIODevice::Append) ) {
      QWriteLocker locker( &mLock );
      SdLibraryHeader hdr;
      item->getHeader( hdr );
      QString key = item->getUid();
      SdLibraryReference ref;
      //write header first
      ref.mHeaderPtr     = fileHdr.size();
      ref.mCreationTime  = hdr.mTime;
      //Only for owner objects we allow uploading to server
      ref.mFlags         = item->getAuthor() == item->getDefaultAuthor() ? SDLR_NEED_UPLOAD : 0;
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
//! \param fileName Library file fileName
//! \return         Full path to library file
//!
QString SdLibraryStorage::fullPath(const QString &fileName) const
  {
  SvDir dir( mLibraryPath );
  return dir.slashedPath() + fileName;
  }




//!
//! \brief fileNameOfLibraryObject Returns file name of library object
//! \param uid                     Unical id of object
//! \return                        File name of library object
//!
QString SdLibraryStorage::fileNameOfLibraryObject(const QString &uid) const
  {
  return SdContainerFile::getLibraryFName( uid, mReferenceMap.value(uid).mCreationTime );
  }




//!
//! \brief fullPathOfLibraryObject Returns full path to object in library
//! \param uid                     Unical id of object
//! \return                        Full path to file of object in library
//!
QString SdLibraryStorage::fullPathOfLibraryObject(const QString &uid) const
  {
  return fullPath( fileNameOfLibraryObject(uid) );
  }




QString SdLibraryStorage::cachePath()
  {
  SvDir dir( QDir::homePath() );
  dir.cd( QStringLiteral("SaliLAB/SaliCAD/cache/") );
  return dir.slashedPath();
  }










