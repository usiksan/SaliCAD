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
#include <QJsonArray>
#include <QJsonDocument>
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
    mUserList.clear();
    QJsonObject obj = QJsonDocument::fromJson( file.readAll() ).object();
    for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
      SdCsAuthor author;
      author.read( i.value().toObject() );
      mUserList.insert( i.key(), author );
      }

//    QDataStream is( &file );
//    is >> mUserList;
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
      QJsonObject obj;
      for( auto i = mUserList.constBegin(); i != mUserList.constEnd(); i++ )
        obj.insert( i.key(), i.value().write() );
      file.write( QJsonDocument(obj).toJson() );
//      QDataStream os( &file );
//      os << mUserList;
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



QJsonObject SdCsAuthor::write() const
  {
  QJsonObject obj;
  obj.insert( QStringLiteral("email"), mEmail );
  obj.insert( QStringLiteral("remain object"), mRemainObject );
  obj.insert( QStringLiteral("max machines"), mMaxMachines );
  QJsonArray arr;
  for( quint64 key : mMachineKeys )
    arr.append( QJsonValue( QString::number(key) )   );
  obj.insert( QStringLiteral("keys"), arr );

  return obj;
  }




void SdCsAuthor::read(const QJsonObject &obj)
  {
  mEmail        = obj.value( QStringLiteral("email") ).toString();
  mRemainObject = obj.value( QStringLiteral("remain object") ).toInt();
  mMaxMachines  = obj.value( QStringLiteral("max machines") ).toInt();

  mMachineKeys.clear();
  QJsonArray arr = obj.value( QStringLiteral("keys") ).toArray();
  for( auto i = arr.constBegin(); i != arr.constEnd(); i++ )
    mMachineKeys.insert( i->toString().toULong() );
  }
