/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdCsAuthorTable.h"

#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QDataStream>
#include <QSaveFile>
#include <QRandomGenerator64>
#include <QDebug>

SdCsAuthorTable::SdCsAuthorTable() :
  mDirty(false)
  {

  }

bool SdCsAuthorTable::registerAuthor(const QString author, const QString email, int maxMachines, int maxObject, quint64 *key)
  {
  QWriteLocker locker(&mLock);
  //Test if user exist return false
  if( mUserList.contains(author) ) {
    *key = 0;
    return false;
    }

  SdCsAuthor user;
  user.mEmail        = email;
  user.mMaxMachines  = maxMachines;
  user.mRemainObject = maxObject;

  mUserList.insert( author, user );
  mDirty = true;

  return registerMachinePrivate( author, key );
  }




bool SdCsAuthorTable::registerMachine(const QString author, quint64 *key)
  {
  QWriteLocker locker(&mLock);

  return registerMachinePrivate( author, key );
  }




bool SdCsAuthorTable::login(const QString author, quint64 key)
  {
  QReadLocker locker(&mLock);
  return mUserList.contains(author) && mUserList.value(author).mMachineKeys.contains(key);
  }




int SdCsAuthorTable::remainObject(const QString author)
  {
  QReadLocker locker(&mLock);
  return mUserList.contains(author) ? mUserList.value(author).mRemainObject : 0;
  }




int SdCsAuthorTable::decrementObject(const QString author)
  {
  int remain = remainObject( author );
  if( remain == 0 ) return 0;
  QWriteLocker locker(&mLock);
  mUserList[author].mRemainObject--;
  mDirty = true;
  return mUserList[author].mRemainObject;
  }




void SdCsAuthorTable::load(const QString path)
  {
  mPath = path;
  QFile file(path);
  if( file.open(QIODevice::ReadOnly) ) {
    QDataStream is( &file );
    is >> mUserList;
    mDirty = false;
    }
  }




void SdCsAuthorTable::save()
  {
  QReadLocker locker(&mLock);

  //qDebug() << "save" << mDirty << mPath;
  if( mDirty ) {
    QFileInfo info(mPath);
    QDir dir(info.absoluteDir());
    dir.mkpath( info.absolutePath() );

    QSaveFile file(mPath);
    if( file.open(QIODevice::WriteOnly) ) {
      QDataStream os( &file );
      os << mUserList;
      mDirty = false;
      file.commit();
      }
    }
  }




bool SdCsAuthorTable::registerMachinePrivate(const QString author, quint64 *key)
  {
  static QRandomGenerator64 gen;
  if( !mUserList.contains(author) || mUserList[author].mMaxMachines <= mUserList[author].mMachineKeys.count() )
    return false;

  *key = gen.generate();
  mUserList[author].mMachineKeys.insert( (*key) );
  mDirty = true;
  return true;
  }


