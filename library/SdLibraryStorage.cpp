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

#include <QDir>
#include <QReadLocker>
#include <QWriteLocker>
#include <QSaveFile>
#include <QFileInfo>
#include <QDebug>

#define FNAME_REF mPath + QStringLiteral("reference.dat")
#define FNAME_HDR mPath + QStringLiteral("headers.dat")
#define FNAME_OBJ mPath + QStringLiteral("objects.dat")
#define HDR_START 18

SdLibraryStorage::SdLibraryStorage() :
  mPath(),
  mLock(),
  mCreationIndex(0),
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
    is >> mCreationIndex >> mReferenceMap;
    }
  else mPath.clear();
  }





//Return true if object referenced in map
bool SdLibraryStorage::contains(const QString uid)
  {
  QReadLocker locker( &mLock );
  return mReferenceMap.contains( uid );
  }




//Return true if object contained in map
bool SdLibraryStorage::isObjectContains(const QString uid)
  {
  QReadLocker locker( &mLock );
  return mReferenceMap.contains( uid ) && mReferenceMap.value(uid).mObjectPtr != 0;
  }





//Return true if object referenced in map and object is newer or same or if newer reference
bool SdLibraryStorage::isNewerOrSameObject(const QString uid, qint32 time)
  {
  QReadLocker locker( &mLock );
  return mReferenceMap.contains( uid ) && mReferenceMap.value(uid).isObjectNewerOrSame( time );
  }





//Return true if newer object referenced in map
bool SdLibraryStorage::isNewerObject(const QString uid, qint32 time)
  {
  QReadLocker locker( &mLock );
  return mReferenceMap.contains( uid ) && mReferenceMap.value(uid).isNewer( time );
  }




//Get list of objects inserted after index
QStringList SdLibraryStorage::getAfter(qint32 index, int limit)
  {
  QReadLocker locker( &mLock );
  QStringList result;
  //if index greater or equal creationIndex then no objects after index
  if( index >= mCreationIndex )
    return result;
  QMapIterator<QString,SdLibraryReference> iter( mReferenceMap );
  qint32 last = index + limit;
  while( iter.hasNext() ) {
    iter.next();
    //If object inside index bound and present not only head but object itsels
    // then append such object to list
    if( iter.value().mCreationIndex >= index && iter.value().mCreationIndex < last && iter.value().mObjectPtr )
      result.append( iter.key() );
    }
  return result;
  }



//Execute function for each object header
bool SdLibraryStorage::forEachHeader(std::function<bool(SdLibraryHeader&)> fun1)
  {
  QReadLocker locker( &mLock );
  mHeaderFile.seek( HDR_START );
  QDataStream is( &mHeaderFile );
  SdLibraryHeader hdr;
  while( !mHeaderFile.atEnd() ) {
    hdr.read( is );
    //qDebug() << hdr.uid() << hdr.mTime;
    //Test if header not deleted
    if( mReferenceMap.value(hdr.uid()).mCreationTime == hdr.mTime ) {
      if( fun1( hdr) )
        return true;
      }
    }
  return false;
  }



//if( (hdr.mClass & mask) && mReferenceMap.value(hdr.uid()).mCreationTime == hdr.mTime ) {
//  if( expandVariant && hdr.variantTableExist() ) {
//    int c = hdr.variantCount();
//    SdLibraryHeader vhdr;
//    for( int i = 0; i < c; i++ ) {
//      hdr.variant( vhdr, i );
//      if( fun1( vhdr ) )
//        return true;
//      }
//    }





//Get header of object
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






//Set reference to object with header
void SdLibraryStorage::setHeader(SdLibraryHeader &hdr)
  {
  QWriteLocker locker( &mLock );

  QString key = hdr.uid();
  //If object already in base then do nothing
  if( mReferenceMap.contains(key) && mReferenceMap.value(key).isNewerOrSame(hdr.mTime) )
    //Newer or same header already present in base
    //Nothing done
    return;

  //Else insert record
  QFile file(FNAME_HDR);
  if( file.open(QIODevice::Append) ) {
    SdLibraryReference ref;
    ref.mHeaderPtr     = file.size();
    ref.mCreationIndex = mCreationIndex++;
    ref.mObjectPtr     = 0;
    ref.mCreationTime  = hdr.mTime;

    QDataStream os( &file );
    hdr.write( os );
    file.close();

    mReferenceMap.insert( key, ref );
    mDirty = true;
    }
  }






//Get object
QByteArray SdLibraryStorage::object(const QString uid)
  {
  QReadLocker locker( &mLock );
  if( !mReferenceMap.contains(uid) || mReferenceMap.value(uid).mObjectPtr == 0 )
    return QByteArray();

  mObjectFile.seek( mReferenceMap.value(uid).mObjectPtr );
  QDataStream is( &mObjectFile );
  QByteArray res;
  is >> res;
  return res;
  }





//Insert new object with creation reference and append header and object
void SdLibraryStorage::insert(const SdLibraryHeader &hdr, QByteArray obj)
  {
  QWriteLocker locker( &mLock );

  QString key = hdr.uid();

  if( mReferenceMap.contains(key) && mReferenceMap.value(key).isObjectNewerOrSame(hdr.mTime) )
    //Newer or same object already present in database
    return;

  QFile fileObj(FNAME_OBJ);
  QFile fileHdr(FNAME_HDR);
  if( fileObj.open(QIODevice::Append) && fileHdr.open(QIODevice::Append) ) {
    SdLibraryReference ref;
    if( !mReferenceMap.contains(key) || mReferenceMap.value(key).mCreationTime != hdr.mTime ) {
      //No record with this name and time
      //write header first
      ref.mCreationIndex = mCreationIndex++;
      ref.mHeaderPtr     = fileHdr.size();
      ref.mCreationTime  = hdr.mTime;
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
      os << mCreationIndex << mReferenceMap;
      if( file.commit() )
        mDirty = false;
      }
    }
  }















