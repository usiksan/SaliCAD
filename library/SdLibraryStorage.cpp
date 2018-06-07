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
#include "objects/SdUtil.h"

#include <QDir>
#include <QReadLocker>
#include <QWriteLocker>
#include <QSaveFile>
#include <QFileInfo>

#define FNAME_REF mPath + QStringLiteral("reference.dat")
#define FNAME_HDR mPath + QStringLiteral("headers.dat")
#define FNAME_OBJ mPath + QStringLiteral("objects.dat")
#define HDR_START 18

SdLibraryStorage::SdLibraryStorage() :
  mPath(),
  mLock(),
  mReferenceMap(),
  mHeaderFile(),
  mObjectFile(),
  mDirty(false)
  {

  }

SdLibraryStorage::~SdLibraryStorage()
  {
  flush();
  }



//With settings library path Storage creates files if yet not created, open files and load map
void SdLibraryStorage::setLibraryPath(const QString path)
  {
  QWriteLocker locker( &mLock );

  //Close previously opened data base
  if( mHeaderFile.isOpen() ) {
    mHeaderFile.close();
    mObjectFile.close();
    }

  //Setup new path
  mPath = path;
  if( mPath == QString("/") ) {
    //library in root unavailable
    mPath.clear();
    return;
    }

  //Check back slash
  if( !mPath.endsWith( QChar('/') ) )
    mPath.append( QChar('/') );

  //mReferenceFile.setFileName( FNAME_REF );
  mHeaderFile.setFileName( FNAME_HDR );
  mObjectFile.setFileName( FNAME_OBJ );

  //Check if directory present
  QDir dir(mPath);
  if( !dir.exists() ) {
    //Directory not exist, create one
    if( !dir.mkpath( mPath ) ) {
      mPath.clear();
      return;
      }
    //Create data files
    QFile file(FNAME_REF);
    if( !file.open( QIODevice::WriteOnly ) ) {
      mPath.clear();
      return;
      }
    file.close();

    if( !mHeaderFile.open( QIODevice::WriteOnly ) ) {
      mPath.clear();
      return;
      }
    mHeaderFile.write( "saliLibraryHeaders", HDR_START );
    mHeaderFile.close();

    if( !mObjectFile.open( QIODevice::WriteOnly ) ) {
      mPath.clear();
      return;
      }
    mObjectFile.write( "saliLibraryObjects", 18 );
    mObjectFile.close();
    }

  //Check if file present
  if( !QFile::exists(FNAME_REF) || !QFile::exists(FNAME_HDR) || !QFile::exists(FNAME_OBJ) ) {
    //One or more files are not exists - work is fail
    mPath.clear();
    return;
    }

  //Open headers and objects files
  if( !mHeaderFile.open( QIODevice::ReadOnly ) || !mObjectFile.open( QIODevice::ReadOnly ) )
    mPath.clear();

  //Read reference file
  QFile file(FNAME_REF);
  if( file.open(QIODevice::ReadOnly) ) {
    QDataStream is( &file );
    is >> mReferenceMap;
    }
  else mPath.clear();
  }





//Return true if object referenced in map
bool SdLibraryStorage::contains(const QString key)
  {
  QReadLocker locker( &mLock );
  return mReferenceMap.contains( key );
  }




//Return true if object contained in map
bool SdLibraryStorage::isObjectContains(const QString key)
  {
  QReadLocker locker( &mLock );
  return mReferenceMap.contains( key ) && mReferenceMap.value(key).mObjectPtr != 0;
  }




//Get list of objects older time
QStringList SdLibraryStorage::getOlder(qint32 time, int limit)
  {
  QReadLocker locker( &mLock );
  QStringList result;
  QMapIterator<QString,SdLibraryReference> iter( mReferenceMap );
  int i = 0;
  while( iter.hasNext() ) {
    iter.next();
    if( iter.value().mTime >= time ) {
      result.append( iter.key() );
      if( ++i >= limit ) break;
      }
    }
  return result;
  }




bool SdLibraryStorage::forEachHeader(std::function<bool(SdLibraryHeader&)> fun1)
  {
  QReadLocker locker( &mLock );
  mHeaderFile.seek( HDR_START );
  QDataStream is( &mHeaderFile );
  SdLibraryHeader hdr;
  while( !mHeaderFile.atEnd() ) {
    hdr.read( is );
    if( fun1( hdr) )
      return true;
    }
  return false;
  }





//Get header of object
bool SdLibraryStorage::header(const QString key, SdLibraryHeader &hdr)
  {
  //For empty key return false to indicate no header
  if( key.isEmpty() ) return false;

  QWriteLocker locker( &mLock );
  if( !mReferenceMap.contains(key) ) return false;
  mHeaderFile.seek( mReferenceMap.value(key).mHeaderPtr );
  QDataStream is( &mHeaderFile );
  hdr.read( is );
  return true;
  }






//Set reference to object with header
void SdLibraryStorage::setHeader(const QString key, SdLibraryHeader &hdr)
  {
  QWriteLocker locker( &mLock );
  if( mReferenceMap.contains(key) )
    return;
  QFile file(FNAME_HDR);
  if( file.open(QIODevice::Append) ) {
    SdLibraryReference ref;
    ref.mHeaderPtr = file.size();
    ref.mTime      = SdUtil::getTime2000();
    ref.mObjectPtr = 0;

    QDataStream os( &file );
    hdr.write( os );
    file.close();

    mReferenceMap.insert( key, ref );
    mDirty = true;
    }
  }






//Get object
QByteArray SdLibraryStorage::object(const QString key)
  {
  QReadLocker locker( &mLock );
  if( !mReferenceMap.contains(key) || mReferenceMap.value(key).mObjectPtr == 0 )
    return QByteArray();

  mObjectFile.seek( mReferenceMap.value(key).mObjectPtr );
  QDataStream is( &mObjectFile );
  QByteArray res;
  is >> res;
  return res;
  }





//Set object to reference. Reference must be exist
void SdLibraryStorage::setObject(const QString key, QByteArray obj)
  {
  QWriteLocker locker( &mLock );

  if( !mReferenceMap.contains(key) )
    return;
  QFile file(FNAME_OBJ);
  if( file.open(QIODevice::Append) ) {
    SdLibraryReference ref( mReferenceMap.value(key) );
    ref.mObjectPtr = file.size();

    QDataStream os( &file );
    os << obj;
    file.close();

    mReferenceMap.insert( key, ref );
    mDirty = true;
    }
  }




//Insert new object with creation reference and append header and object
void SdLibraryStorage::insert(const QString key, const SdLibraryHeader &hdr, QByteArray obj)
  {
  QWriteLocker locker( &mLock );

  if( mReferenceMap.contains(key) && mReferenceMap.value(key).mObjectPtr != 0 )
    return;
  QFile fileObj(FNAME_OBJ);
  QFile fileHdr(FNAME_HDR);
  if( fileObj.open(QIODevice::Append) && fileHdr.open(QIODevice::Append) ) {
    SdLibraryReference ref;
    if( !mReferenceMap.contains(key) ) {
      //No record with this name
      //write header first
      ref.mTime      = SdUtil::getTime2000();
      ref.mHeaderPtr = fileHdr.size();
      QDataStream os( &fileHdr );
      hdr.write( os );
      fileHdr.close();
      }
    else ref = mReferenceMap.value(key);
    //Header info already present
    //write obj
    ref.mObjectPtr = fileObj.size();

    QDataStream os( &fileObj );
    os << obj;
    fileObj.close();

    mReferenceMap.insert( key, ref );
    mDirty = true;
    }
  }





//Flush reference map if needed
void SdLibraryStorage::flush()
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
  }











